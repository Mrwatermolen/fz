// #ifndef __FZ_IO_H__
// #define __FZ_IO_H__

// #include <ostream>

// #include "array.hpp"

// namespace fz {

// // template <typename T>
// // inline auto operator<<(std::ostream& os, const Array<T>& arr) -> std::ostream& {
// //   auto shape = arr.shape();
// //   auto n_dim = shape.size();
// //   if (n_dim == 0) {
// //     os << "[]";
// //     return os;
// //   }
// //   os << "[";
// //   for (auto i = 0; i < shape[0]; ++i) {
// //     if (n_dim == 1) {
// //       os << arr(i);
// //     } else {
// //       os << arr[i];
// //     }
// //     if (i < shape[0] - 1) {
// //       os << ", ";
// //     }
// //   }
// //   os << "]";

// //   return os;
// // }

// }  // namespace fz

// #endif  // __FZ_IO_H__
