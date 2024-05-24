// #ifndef __FZ_ALLOCATOR_H__
// #define __FZ_ALLOCATOR_H__

// #include <cstddef>
// #include <iostream>
// #include <limits>
// #include <new>
// #include <sstream>

// #include "fz/memory.hpp"

// namespace fz {

// #if FZ_DEBUG
// #define FZ_DEBUG_PRINT(x) std::cout << (x) << '\n'
// #else
// #define FZ_DEBUG_PRINT(x)
// #endif

// template <typename T>
// class Allocator {
//  public:
//   using value_type = T;
//   using pointer = T*;
//   using const_pointer = const T*;
//   using reference = T&;
//   using const_reference = const T&;
//   using size_type = std::size_t;
//   using difference_type = std::ptrdiff_t;

//   template <typename U>
//   struct Rebind {
//     using other = Allocator<U>;
//   };

//  public:
//   Allocator();

//   Allocator(const Allocator& other);

//   Allocator(Allocator&& other) noexcept;

//   ~Allocator() = default;

//  public:
//   // basic functions

//   /**
//    * @brief Allocate memory for n objects of type T
//    *
//    * @param n: The number of objects to allocate memory for
//    * @param hint: A pointer to a nearby memory location that might help the
//    * @return pointer: A pointer to the first object in the allocated memory
//    */
//   auto allocate(size_type n, const void* hint = nullptr) -> pointer;

//   /**
//    * @brief Deallocate memory for n objects of type T
//    *
//    * @param p: A pointer to the first object in the allocated memory
//    * @param n: The number of objects to deallocate memory for
//    */
//   auto deallocate(pointer p, size_type n) -> void;

//   /**
//    * @brief Deconstruct the object pointed to by p
//    *
//    * @param p: A pointer to the object to destroy
//    */
//   auto destroy(pointer p) -> void;

//  public:
//   // template <typename... Args>
//   // auto constructAt(pointer p, Args&&... args) -> void;

//   // [[nodiscard]] auto maxSize() const -> size_type;

//   // [[nodiscard]] auto address(reference x) const -> pointer;

//   // [[nodiscard]] auto constAddress(const_reference x) const -> const_pointer;

//  private:
//   static auto fzAllocate(size_type n, T* hint) -> pointer;

//   static auto fzDeallocate(pointer p) -> void;

//   template <typename... Args>
//   static auto fzConstructAt(pointer p, Args&&... args) -> void;

//   static auto fzDestroy(pointer p) -> void;
// };

// template <typename T>
// auto Allocator<T>::fzAllocate(size_type n, T* hint) -> pointer {
//   std::stringstream ss;
//   ss << "Allocating memory for " << n << " objects of type "
//      << typeid(T).name();
//   FZ_DEBUG_PRINT(ss.str());
//   std::set_new_handler(nullptr);
//   auto p = MallocAlloc::allocate(n);

//   ss.str("");
//   ss << "Memory allocated at " << p;
//   FZ_DEBUG_PRINT(ss.str());
//   return static_cast<pointer>(p);
// }

// template <typename T>
// void Allocator<T>::fzDeallocate(pointer p) {
//   std::stringstream ss;
//   ss << "Deallocating memory at " << p;
//   FZ_DEBUG_PRINT(ss.str());
//   MallocAlloc::deallocate(p);
// }

// template <typename T>
// template <typename... Args>
// void Allocator<T>::fzConstructAt(pointer p, Args&&... args) {
//   std::stringstream ss;
//   ss << "Constructing object at " << p;
//   FZ_DEBUG_PRINT(ss.str());
//   fz::construct(p, std::forward<Args>(args)...);
// }

// template <typename T>
// void Allocator<T>::fzDestroy(pointer p) {
//   std::stringstream ss;
//   ss << "Destroying object at " << p;
//   FZ_DEBUG_PRINT(ss.str());
//   fz::destroy(p);
// }

// template <typename T>
// Allocator<T>::Allocator() = default;

// template <typename T>
// Allocator<T>::Allocator(const Allocator& other) = default;

// template <typename T>
// Allocator<T>::Allocator(Allocator&& other) noexcept = default;

// template <typename T>
// auto Allocator<T>::allocate(size_type n, const void* hint) -> pointer {
//   return fzAllocate(n, static_cast<T*>(const_cast<void*>(hint)));
// }

// template <typename T>
// auto Allocator<T>::deallocate(pointer p, size_type n) -> void {
//   fzDeallocate(p);
// }

// template <typename T>
// auto Allocator<T>::destroy(pointer p) -> void {
//   fzDestroy(p);
// }

// // template <typename T>
// // template <typename... Args>
// // auto Allocator<T>::constructAt(pointer p, Args&&... args) -> void {
// //   fzConstructAt(p, std::forward<Args>(args)...);
// // }

// // template <typename T>
// // auto Allocator<T>::maxSize() const -> size_type {
// //   return std::numeric_limits<size_type>::max() / sizeof(T);
// // }

// // template <typename T>
// // auto Allocator<T>::address(reference x) const -> pointer {
// //   return &x;
// // }

// // template <typename T>
// // auto Allocator<T>::constAddress(const_reference x) const -> const_pointer {
// //   return &x;
// // }

// }  // namespace fz

// #endif  // __FZ_ALLOCATOR_H__
