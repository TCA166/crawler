
#pragma once

#include "../gl/texture.hpp"

/*!
 @brief An interface for a class that can see. Different kinds of lights can
  implement this interface to be used in the scene.
*/
class view {
public:
  /*!
   @brief Bind the view map to the framebuffer
  */
  virtual void bind_view_map() const = 0;
  /*!
   @brief Get the view map of the object as a texture
   @return The view map of the object
   @warning Performance heavy
  */
  virtual texture *get_view_map() const = 0;
  virtual ~view() {}
};
