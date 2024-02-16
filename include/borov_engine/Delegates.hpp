/**
DOCUMENTATION

// CONFIG

Override default asset
#define DELEGATE_ASSERT(expression, ...)

Override default static_assert
#define DELEGATE_STATIC_ASSERT(expression, msg)

Set inline allocator size (default: 32)
constexpr std::size_t kDelegateInlineAllocationSize = ...;

Reassign allocation functions:
Delegates::SetAllocationCallbacks(allocFunction, freeFunc);


// USAGE

## Classes ##
- ```Delegate<RetVal, Args>```
- ```MulticastDelegate<Args>```

## Features ##
- Support for:
	- Static/Global methods
	- Member functions
	- Lambda's
	- std::shared_ptr
- Delegate object is allocated inline if it is under 32 bytes
- Add payload to delegate during bind-time
- Move operations enable optimization

## Example Usage ##

### Delegate ###

Delegate<int, float> del;
del.BindLambda([](float a, int payload)
{
	std::cout << "Lambda delegate parameter: " << a << std::endl;
	std::cout << "Lambda delegate payload: " << payload << std::endl;
	return 10;
}, 50);
std::cout << "Lambda delegate return value: " << del.Execute(20) << std::endl;

Output:
Lambda delegate parameter: 20
Lambda delegate payload: 50
Lambda delegate return value: 10

### MulticastDelegate ###

struct Foo
{
	void Bar(float a, int payload)
	{
		std::cout << "Raw delegate parameter: " << a << std::endl;
		std::cout << "Raw delegate payload: " << payload << std::endl;
	}
};
MulticastDelegate<float> del;
del.AddLambda([](float a, int payload)
{
	std::cout << "Lambda delegate parameter: " << a << std::endl;
	std::cout << "Lambda delegate payload: " << payload << std::endl;
}, 90);

Foo foo;
del.AddRaw(&foo, &Foo::Bar, 10);
del.Broadcast(20);

Output:
Lambda delegate parameter: 20
Lambda delegate payload: 90
Raw delegate parameter: 20
Raw delegate payload: 10

*/

#pragma once

#include <vector>
#include <memory>
#include <tuple>
#include <cassert>

// The allocation size of delegate data.
// Delegates larger than this will be heap allocated.
constexpr std::size_t kDelegateInlineAllocationSize = 32;

///////////////////////////////////////////////////////////////
//////////////////// DEFINES SECTION //////////////////////////
///////////////////////////////////////////////////////////////

#define DECLARE_DELEGATE(name, ...) \
using name = Delegate<void, __VA_ARGS__>

#define DECLARE_DELEGATE_RET(name, retValue, ...) \
using name = Delegate<retValue, __VA_ARGS__>

#define DECLARE_MULTICAST_DELEGATE(name, ...) \
using name = MulticastDelegate<__VA_ARGS__>; \
using name ## Delegate = MulticastDelegate<__VA_ARGS__>::DelegateT

#define DECLARE_EVENT(name, ownerType, ...) \
class name : public MulticastDelegate<__VA_ARGS__> \
{ \
private: \
    friend class ownerType; \
    using MulticastDelegate::Broadcast; \
    using MulticastDelegate::RemoveAll; \
    using MulticastDelegate::Remove; \
};

///////////////////////////////////////////////////////////////
/////////////////// INTERNAL SECTION //////////////////////////
///////////////////////////////////////////////////////////////

namespace detail {

template<bool IsConst, typename Object, typename RetVal, typename ...Args>
struct MemberFunction;

template<typename Object, typename RetVal, typename ...Args>
struct MemberFunction<true, Object, RetVal, Args...> {
    using Type = RetVal(Object::*)(Args...) const;
};

template<typename Object, typename RetVal, typename ...Args>
struct MemberFunction<false, Object, RetVal, Args...> {
    using Type = RetVal(Object::*)(Args...);
};

static void *(*Alloc)(size_t size) = [](size_t size) { return malloc(size); };
static void (*Free)(void *pPtr) = [](void *pPtr) { free(pPtr); };

template<typename T>
void DelegateDeleteFunc(T *pPtr) {
    pPtr->~T();
    DelegateFreeFunc(pPtr);
}

}

namespace Delegates {

using AllocateCallback = void *(*)(size_t size);
using FreeCallback = void (*)(void *pPtr);

inline void SetAllocationCallbacks(AllocateCallback allocateCallback, FreeCallback freeCallback) {
    detail::Alloc = allocateCallback;
    detail::Free = freeCallback;
}

}

class IDelegateBase {
  public:
    IDelegateBase() = default;
    virtual ~IDelegateBase() noexcept = default;
    [[nodiscard]] virtual const void *GetOwner() const { return nullptr; }
};

// Base type for delegates
template<typename RetVal, typename... Args>
class IDelegate : public IDelegateBase {
  public:
    [[nodiscard]] virtual RetVal Execute(Args &&... args) = 0;
};

template<typename RetVal, typename... Args2>
class StaticDelegate;

template<typename RetVal, typename... Args, typename... Args2>
class StaticDelegate<RetVal(Args...), Args2...> : public IDelegate<RetVal, Args...> {
  public:
    using DelegateFunction = RetVal(*)(Args..., Args2...);

    explicit StaticDelegate(DelegateFunction function, Args2 &&... args)
        : function_(function),
          payload_(std::forward<Args2>(args)...) {}

    RetVal Execute(Args &&... args) override {
        return Execute_Internal(std::forward<Args>(args)..., std::index_sequence_for<Args2...>());
    }

  private:
    template<std::size_t... Is>
    RetVal Execute_Internal(Args &&... args, std::index_sequence<Is...>) {
        return function_(std::forward<Args>(args)..., std::get<Is>(payload_)...);
    }

    DelegateFunction function_;
    std::tuple<Args2...> payload_;
};

template<bool IsConst, typename T, typename RetVal, typename... Args2>
class RawDelegate;

template<bool IsConst, typename T, typename RetVal, typename... Args, typename... Args2>
class RawDelegate<IsConst, T, RetVal(Args...), Args2...> : public IDelegate<RetVal, Args...> {
  public:
    using DelegateFunction = typename detail::MemberFunction<IsConst, T, RetVal, Args..., Args2...>::Type;

    RawDelegate(T *pObject, DelegateFunction function, Args2 &&... args)
        : object_(pObject),
          function_(function),
          payload_(std::forward<Args2>(args)...) {}

    RetVal Execute(Args &&... args) override {
        return Execute_Internal(std::forward<Args>(args)..., std::index_sequence_for<Args2...>());
    }

    [[nodiscard]] const void *GetOwner() const override {
        return object_;
    }

  private:
    template<std::size_t... Is>
    RetVal Execute_Internal(Args &&... args, std::index_sequence<Is...>) {
        return (object_->*function_)(std::forward<Args>(args)..., std::get<Is>(payload_)...);
    }

    T *object_;
    DelegateFunction function_;
    std::tuple<Args2...> payload_;
};

template<typename TLambda, typename RetVal, typename... Args>
class LambdaDelegate;

template<typename TLambda, typename RetVal, typename... Args, typename... Args2>
class LambdaDelegate<TLambda, RetVal(Args...), Args2...> : public IDelegate<RetVal, Args...> {
  public:
    explicit LambdaDelegate(TLambda &&lambda, Args2 &&... args)
        : lambda_(std::forward<TLambda>(lambda)),
          payload_(std::forward<Args2>(args)...) {}

    RetVal Execute(Args &&... args) override {
        return Execute_Internal(std::forward<Args>(args)..., std::index_sequence_for<Args2...>());
    }

  private:
    template<std::size_t... Is>
    RetVal Execute_Internal(Args &&... args, std::index_sequence<Is...>) {
        return (RetVal) ((lambda_)(std::forward<Args>(args)..., std::get<Is>(payload_)...));
    }

    TLambda lambda_;
    std::tuple<Args2...> payload_;
};

template<bool IsConst, typename T, typename RetVal, typename... Args>
class SPDelegate;

template<bool IsConst, typename RetVal, typename T, typename... Args, typename... Args2>
class SPDelegate<IsConst, T, RetVal(Args...), Args2...> : public IDelegate<RetVal, Args...> {
  public:
    using DelegateFunction = typename detail::MemberFunction<IsConst, T, RetVal, Args..., Args2...>::Type;

    SPDelegate(const std::shared_ptr<T> &pObject, DelegateFunction pFunction, Args2 &&... args)
        : object_(pObject),
          function_(pFunction),
          payload_(std::forward<Args2>(args)...) {}

    RetVal Execute(Args &&... args) override {
        return Execute_Internal(std::forward<Args>(args)..., std::index_sequence_for<Args2...>());
    }

    [[nodiscard]] const void *GetOwner() const override {
        return object_.expired() ? nullptr : object_.lock().get();
    }

  private:
    template<std::size_t... Is>
    RetVal Execute_Internal(Args &&... args, std::index_sequence<Is...>) {
        if (object_.expired()) {
            return RetVal();
        } else {
            std::shared_ptr<T> pPinned = object_.lock();
            return (pPinned.get()->*function_)(std::forward<Args>(args)..., std::get<Is>(payload_)...);
        }
    }

    std::weak_ptr<T> object_;
    DelegateFunction function_;
    std::tuple<Args2...> payload_;
};

// A handle to a delegate used for a multicast delegate
// Static ID so that every handle is unique
class DelegateHandle {
  public:
    constexpr DelegateHandle() noexcept
        : id_(INVALID_ID) {
    }

    explicit DelegateHandle([[maybe_unused]] bool generateId) noexcept
        : id_(GetNewID()) {
    }

    ~DelegateHandle() noexcept = default;
    DelegateHandle(const DelegateHandle &other) = default;
    DelegateHandle &operator=(const DelegateHandle &other) = default;

    DelegateHandle(DelegateHandle &&other) noexcept
        : id_(other.id_) {
        other.Reset();
    }

    DelegateHandle &operator=(DelegateHandle &&other) noexcept {
        id_ = other.id_;
        other.Reset();
        return *this;
    }

    explicit operator bool() const noexcept {
        return IsValid();
    }

    bool operator==(const DelegateHandle &other) const noexcept {
        return id_ == other.id_;
    }

    bool operator<(const DelegateHandle &other) const noexcept {
        return id_ < other.id_;
    }

    [[nodiscard]] bool IsValid() const noexcept {
        return id_ != INVALID_ID;
    }

    void Reset() noexcept {
        id_ = INVALID_ID;
    }

    constexpr static const unsigned int INVALID_ID = (unsigned int) ~0;

  private:
    static unsigned int CURRENT_ID;

    static unsigned int GetNewID() {
        unsigned int output = DelegateHandle::CURRENT_ID++;
        if (DelegateHandle::CURRENT_ID == INVALID_ID) {
            DelegateHandle::CURRENT_ID = 0;
        }
        return output;
    }

    unsigned int id_;
};

template<size_t MaxStackSize>
class InlineAllocator {
  public:
    constexpr InlineAllocator() noexcept: size_(0), alloc_{} {
        static_assert(MaxStackSize > sizeof(void *), "MaxStackSize is smaller or equal to the size of a pointer. "
                                                     "This will make the use of an InlineAllocator pointless. "
                                                     "Please increase the MaxStackSize.");
    }

    ~InlineAllocator() noexcept {
        Free();
    }

    InlineAllocator(const InlineAllocator &other) : size_(0), alloc_{} {
        if (other.HasAllocation()) {
            memcpy(Allocate(other.size_), other.GetAllocation(), other.size_);
        }
        size_ = other.size_;
    }

    InlineAllocator &operator=(const InlineAllocator &other) {
        if (other.HasAllocation()) {
            memcpy(Allocate(other.size_), other.GetAllocation(), other.size_);
        }
        size_ = other.size_;
        return *this;
    }

    InlineAllocator(InlineAllocator &&other) noexcept: size_(other.size_), alloc_{} {
        other.size_ = 0;
        if (size_ > MaxStackSize) {
            std::swap(alloc_, other.alloc_);
        } else {
            memcpy(buffer_, other.buffer_, size_);
        }
    }

    InlineAllocator &operator=(InlineAllocator &&other) noexcept {
        Free();
        size_ = other.size_;
        other.size_ = 0;
        if (size_ > MaxStackSize) {
            std::swap(alloc_, other.alloc_);
        } else {
            memcpy(buffer_, other.buffer_, size_);
        }
        return *this;
    }

    // Allocate memory of given size
    // If the size is over the predefined threshold, it will be allocated on the heap
    void *Allocate(const size_t size) {
        if (size_ != size) {
            Free();
            size_ = size;
            if (size > MaxStackSize) {
                alloc_ = detail::Alloc(size);
                return alloc_;
            }
        }
        return (void *) buffer_;
    }

    // Free the allocated memory
    void Free() {
        if (size_ > MaxStackSize) {
            detail::Free(alloc_);
        }
        size_ = 0;
    }

    // Return the allocated memory either on the stack or on the heap
    [[nodiscard]] void *GetAllocation() const {
        if (HasAllocation()) {
            return HasHeapAllocation() ? alloc_ : (void *) buffer_;
        } else {
            return nullptr;
        }
    }

    [[nodiscard]] std::size_t GetSize() const {
        return size_;
    }

    [[nodiscard]] bool HasAllocation() const {
        return size_ > 0;
    }

    [[nodiscard]] bool HasHeapAllocation() const {
        return size_ > MaxStackSize;
    }

  private:
    // If the allocation is smaller than the threshold, `buffer_` is used
    // Otherwise `alloc_` is used together with a separate dynamic allocation
    union {
        char buffer_[MaxStackSize];
        void *alloc_;
    };
    std::size_t size_;
};

class DelegateBase {
  public:
    constexpr DelegateBase() noexcept: allocator_() {}

    virtual ~DelegateBase() noexcept {
        Release();
    }

    DelegateBase(const DelegateBase &other) : allocator_(other.allocator_) {}

    DelegateBase &operator=(const DelegateBase &other) {
        Release();
        allocator_ = other.allocator_;
        return *this;
    }

    DelegateBase(DelegateBase &&other) noexcept: allocator_(std::move(other.allocator_)) {}

    DelegateBase &operator=(DelegateBase &&other) noexcept {
        Release();
        allocator_ = std::move(other.allocator_);
        return *this;
    }

    // Gets the owner of the delegate
    // Only valid for SPDelegate and RawDelegate.
    // Otherwise, returns nullptr by default
    [[nodiscard]] const void *GetOwner() const {
        if (allocator_.HasAllocation()) {
            return GetDelegate()->GetOwner();
        }
        return nullptr;
    }

    [[nodiscard]] std::size_t GetSize() const {
        return allocator_.GetSize();
    }

    // Clear the bound delegate if it is bound to the given object.
    // Ignored when pObject is a nullptr
    void ClearIfBoundTo(void *pObject) {
        if (pObject != nullptr && IsBoundTo(pObject)) {
            Release();
        }
    }

    // Clear the bound delegate if it exists
    void Clear() {
        Release();
    }

    // If the allocator has a size, it means it's bound to something
    [[nodiscard]] bool IsBound() const {
        return allocator_.HasAllocation();
    }

    bool IsBoundTo(void *pObject) const {
        if (pObject == nullptr || !allocator_.HasAllocation()) {
            return false;
        }
        return GetDelegate()->GetOwner() == pObject;
    }

  protected:
    void Release() {
        if (allocator_.HasAllocation()) {
            GetDelegate()->~IDelegateBase();
            allocator_.Free();
        }
    }

    [[nodiscard]] IDelegateBase *GetDelegate() const {
        return static_cast<IDelegateBase *>(allocator_.GetAllocation());
    }

    // Allocator for the delegate itself.
    // Delegate gets allocated when its is smaller or equal than 64 bytes in size.
    // Can be changed by preference
    InlineAllocator<kDelegateInlineAllocationSize> allocator_;
};

// Delegate that can be bound to by just ONE object
template<typename RetVal, typename... Args>
class Delegate : public DelegateBase {
  private:
    template<typename T, typename... Args2>
    using ConstMemberFunction = typename detail::MemberFunction<true, T, RetVal, Args..., Args2...>::Type;

    template<typename T, typename... Args2>
    using NonConstMemberFunction = typename detail::MemberFunction<false, T, RetVal, Args..., Args2...>::Type;

  public:
    using IDelegateT = IDelegate<RetVal, Args...>;

    // Create delegate using member function
    template<typename T, typename... Args2>
    [[nodiscard]] static Delegate CreateRaw(T *pObj, NonConstMemberFunction<T, Args2...> pFunction, Args2... args) {
        Delegate handler;
        handler.Bind<RawDelegate<false, T, RetVal(Args...), Args2...>>(pObj, pFunction, std::forward<Args2>(args)...);
        return handler;
    }

    template<typename T, typename... Args2>
    [[nodiscard]] static Delegate CreateRaw(T *pObj, ConstMemberFunction<T, Args2...> pFunction, Args2... args) {
        Delegate handler;
        handler.Bind<RawDelegate<true, T, RetVal(Args...), Args2...>>(pObj, pFunction, std::forward<Args2>(args)...);
        return handler;
    }

    // Create delegate using global/static function
    template<typename... Args2>
    [[nodiscard]] static Delegate CreateStatic(RetVal(*pFunction)(Args..., Args2...), Args2... args) {
        Delegate handler;
        handler.Bind<StaticDelegate<RetVal(Args...), Args2...>>(pFunction, std::forward<Args2>(args)...);
        return handler;
    }

    // Create delegate using std::shared_ptr
    template<typename T, typename... Args2>
    [[nodiscard]] static Delegate CreateSP(const std::shared_ptr<T> &pObject,
                                           NonConstMemberFunction<T, Args2...> pFunction,
                                           Args2... args) {
        Delegate handler;
        handler.Bind<SPDelegate<false, T, RetVal(Args...), Args2...>>(pObject, pFunction, std::forward<Args2>(args)...);
        return handler;
    }

    template<typename T, typename... Args2>
    [[nodiscard]] static Delegate CreateSP(const std::shared_ptr<T> &pObject,
                                           ConstMemberFunction<T, Args2...> pFunction,
                                           Args2... args) {
        Delegate handler;
        handler.Bind<SPDelegate<true, T, RetVal(Args...), Args2...>>(pObject, pFunction, std::forward<Args2>(args)...);
        return handler;
    }

    // Create delegate using a lambda
    template<typename TLambda, typename... Args2>
    [[nodiscard]] static Delegate CreateLambda(TLambda &&lambda, Args2... args) {
        Delegate handler;
        handler.Bind<LambdaDelegate<TLambda, RetVal(Args...), Args2...>>(std::forward<TLambda>(lambda),
                                                                         std::forward<Args2>(args)...);
        return handler;
    }

    // Bind a member function
    template<typename T, typename... Args2>
    void BindRaw(T *pObject, NonConstMemberFunction<T, Args2...> pFunction, Args2 &&... args) {
        DELEGATE_STATIC_ASSERT(!std::is_const<T>::value, "Cannot bind a non-const function on a const object");
        *this = CreateRaw<T, Args2...>(pObject, pFunction, std::forward<Args2>(args)...);
    }

    template<typename T, typename... Args2>
    void BindRaw(T *pObject, ConstMemberFunction<T, Args2...> pFunction, Args2 &&... args) {
        *this = CreateRaw<T, Args2...>(pObject, pFunction, std::forward<Args2>(args)...);
    }

    // Bind a static/global function
    template<typename... Args2>
    void BindStatic(RetVal(*pFunction)(Args..., Args2...), Args2 &&... args) {
        *this = CreateStatic<Args2...>(pFunction, std::forward<Args2>(args)...);
    }

    // Bind a lambda
    template<typename LambdaType, typename... Args2>
    void BindLambda(LambdaType &&lambda, Args2 &&... args) {
        *this = CreateLambda<LambdaType, Args2...>(std::forward<LambdaType>(lambda), std::forward<Args2>(args)...);
    }

    // Bind a member function with a shared_ptr object
    template<typename T, typename... Args2>
    void BindSP(std::shared_ptr<T> pObject, NonConstMemberFunction<T, Args2...> pFunction, Args2 &&... args) {
        DELEGATE_STATIC_ASSERT(!std::is_const<T>::value, "Cannot bind a non-const function on a const object");
        *this = CreateSP<T, Args2...>(pObject, pFunction, std::forward<Args2>(args)...);
    }

    template<typename T, typename... Args2>
    void BindSP(std::shared_ptr<T> pObject, ConstMemberFunction<T, Args2...> pFunction, Args2 &&... args) {
        *this = CreateSP<T, Args2...>(pObject, pFunction, std::forward<Args2>(args)...);
    }

    // Execute the delegate with the given parameters
    RetVal Execute(Args... args) const {
        assert(allocator_.HasAllocation() && "Delegate is not bound");
        return ((IDelegateT *) GetDelegate())->Execute(std::forward<Args>(args)...);
    }

    RetVal ExecuteIfBound(Args... args) const {
        if (IsBound()) {
            return ((IDelegateT *) GetDelegate())->Execute(std::forward<Args>(args)...);
        }
        return RetVal();
    }

  private:
    template<typename T, typename... Args3>
    void Bind(Args3 &&... args) {
        Release();
        void *pAlloc = allocator_.Allocate(sizeof(T));
        new(pAlloc) T(std::forward<Args3>(args)...);
    }
};

class MulticastDelegateBase {
  public:
    virtual ~MulticastDelegateBase() = default;
};

// Delegate that can be bound to by MULTIPLE objects
template<typename... Args>
class MulticastDelegate : public MulticastDelegateBase {
  public:
    using DelegateT = Delegate<void, Args...>;

  private:
    struct DelegateHandlerPair {
        DelegateHandle Handle;
        DelegateT Callback;

        DelegateHandlerPair() : Handle(false) {}

        DelegateHandlerPair(const DelegateHandle &handle, const DelegateT &callback)
            : Handle(handle), Callback(callback) {}

        DelegateHandlerPair(const DelegateHandle &handle, DelegateT &&callback)
            : Handle(handle), Callback(std::move(callback)) {}
    };

    template<typename T, typename... Args2>
    using ConstMemberFunction = typename detail::MemberFunction<true, T, void, Args..., Args2...>::Type;

    template<typename T, typename... Args2>
    using NonConstMemberFunction = typename detail::MemberFunction<false, T, void, Args..., Args2...>::Type;

  public:
    constexpr MulticastDelegate() : locks_(0) {
    }

    ~MulticastDelegate() noexcept override = default;

    MulticastDelegate(const MulticastDelegate &other) = default;

    MulticastDelegate &operator=(const MulticastDelegate &other) = default;

    MulticastDelegate(MulticastDelegate &&other) noexcept
        : events_(std::move(other.events_)),
          locks_(std::move(other.locks_)) {}

    MulticastDelegate &operator=(MulticastDelegate &&other) noexcept {
        events_ = std::move(other.events_);
        locks_ = std::move(other.locks_);
        return *this;
    }

    DelegateHandle operator+=(DelegateT &&handler) noexcept {
        return Add(std::forward<DelegateT>(handler));
    }

    bool operator-=(DelegateHandle &handle) {
        return Remove(handle);
    }

    DelegateHandle Add(DelegateT &&handler) noexcept {
        // Favour an empty space over a possible array reallocation
        for (size_t i = 0; i < events_.size(); ++i) {
            if (events_[i].Handle.IsValid() == false) {
                events_[i] = DelegateHandlerPair(DelegateHandle(true), std::move(handler));
                return events_[i].Handle;
            }
        }
        events_.emplace_back(DelegateHandle(true), std::move(handler));
        return events_.back().Handle;
    }

    // Bind a member function
    template<typename T, typename... Args2>
    DelegateHandle AddRaw(T *pObject, NonConstMemberFunction<T, Args2...> pFunction, Args2 &&... args) {
        return Add(DelegateT::CreateRaw(pObject, pFunction, std::forward<Args2>(args)...));
    }

    template<typename T, typename... Args2>
    DelegateHandle AddRaw(T *pObject, ConstMemberFunction<T, Args2...> pFunction, Args2 &&... args) {
        return Add(DelegateT::CreateRaw(pObject, pFunction, std::forward<Args2>(args)...));
    }

    // Bind a static/global function
    template<typename... Args2>
    DelegateHandle AddStatic(void(*pFunction)(Args..., Args2...), Args2 &&... args) {
        return Add(DelegateT::CreateStatic(pFunction, std::forward<Args2>(args)...));
    }

    // Bind a lambda
    template<typename LambdaType, typename... Args2>
    DelegateHandle AddLambda(LambdaType &&lambda, Args2 &&... args) {
        return Add(DelegateT::CreateLambda(std::forward<LambdaType>(lambda), std::forward<Args2>(args)...));
    }

    //Bind a member function with a shared_ptr object
    template<typename T, typename... Args2>
    DelegateHandle AddSP(std::shared_ptr<T> pObject, NonConstMemberFunction<T, Args2...> pFunction, Args2 &&... args) {
        return Add(DelegateT::CreateSP(pObject, pFunction, std::forward<Args2>(args)...));
    }

    template<typename T, typename... Args2>
    DelegateHandle AddSP(std::shared_ptr<T> pObject, ConstMemberFunction<T, Args2...> pFunction, Args2 &&... args) {
        return Add(DelegateT::CreateSP(pObject, pFunction, std::forward<Args2>(args)...));
    }

    // Removes all handles that are bound from a specific object
    // Ignored when pObject is null
    // Note: Only works on Raw and SP bindings
    void RemoveObject(void *pObject) {
        if (pObject != nullptr) {
            for (size_t i = 0; i < events_.size(); ++i) {
                if (events_[i].Callback.GetOwner() == pObject) {
                    if (IsLocked()) {
                        events_[i].Clear();
                    } else {
                        std::swap(events_[i], events_[events_.size() - 1]);
                        events_.pop_back();
                    }
                }
            }
        }
    }

    // Remove a function from the event list by the handle
    bool Remove(DelegateHandle &handle) {
        if (handle.IsValid()) {
            for (size_t i = 0; i < events_.size(); ++i) {
                if (events_[i].Handle == handle) {
                    if (IsLocked()) {
                        events_[i].Callback.Clear();
                    } else {
                        std::swap(events_[i], events_[events_.size() - 1]);
                        events_.pop_back();
                    }
                    handle.Reset();
                    return true;
                }
            }
        }
        return false;
    }

    [[nodiscard]] bool IsBoundTo(const DelegateHandle &handle) const {
        if (handle.IsValid()) {
            for (size_t i = 0; i < events_.size(); ++i) {
                if (events_[i].Handle == handle) {
                    return true;
                }
            }
        }
        return false;
    }

    // Remove all the functions bound to the delegate
    void RemoveAll() {
        if (IsLocked()) {
            for (DelegateHandlerPair &handler : events_) {
                handler.Callback.Clear();
            }
        } else {
            events_.clear();
        }
    }

    void Compress(const size_t maxSpace = 0) {
        if (IsLocked() == false) {
            size_t toDelete = 0;
            for (size_t i = 0; i < events_.size() - toDelete; ++i) {
                if (events_[i].Handle.IsValid() == false) {
                    std::swap(events_[i], events_[toDelete]);
                    ++toDelete;
                }
            }
            if (toDelete > maxSpace) {
                events_.resize(events_.size() - toDelete);
            }
        }
    }

    // Execute all functions that are bound
    void Broadcast(Args ...args) {
        Lock();
        for (size_t i = 0; i < events_.size(); ++i) {
            if (events_[i].Handle.IsValid()) {
                events_[i].Callback.Execute(std::forward<Args>(args)...);
            }
        }
        Unlock();
    }

    [[nodiscard]] std::size_t GetSize() const {
        return events_.size();
    }

  private:
    void Lock() {
        ++locks_;
    }

    void Unlock() {
        // Unlock() should never be called more than Lock()!
        assert(locks_ > 0);
        --locks_;
    }

    // Returns true is the delegate is currently broadcasting
    // If this is true, the order of the array should not be changed otherwise this causes undefined behaviour
    [[nodiscard]] bool IsLocked() const {
        return locks_ > 0;
    }

    std::vector<DelegateHandlerPair> events_;
    unsigned int locks_;
};
