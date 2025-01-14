
#pragma once

#include "../gl/texture.hpp"

/*!
 @brief An interface for a class that can see
*/
class view {
public:
  /*!
   @brief Bind the view map
  */
  virtual void bind_view_map() const = 0;
  /*!
   @brief Get the view map
   @return The view map
  */
  virtual texture *get_view_map() const = 0;
  virtual ~view() {}
};
