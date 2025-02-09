#pragma once
#include "../engine/abc/moveable.hpp"
#include "../engine/abc/view.hpp"
#include "../engine/engine.hpp"
#include "../engine/gl/shader.hpp"
#include "../engine/gl/texture.hpp"
#include "../engine/utils/model_loader.hpp"
#include <glm/glm.hpp>
#include <string>

#define GUN_RECOIL_STRENGTH 0.9f
#define GUN_RECOIL_RECOVERY_SPEED 5.0f
#define GUN_SCALE 0.1f         // Scale the gun model down to reasonable size
#define GUN_VIEW_DISTANCE 3.0f // How far in front of camera
#define GUN_VIEW_DROP 2.0f 
#define GUN_HORIZONTAL_OFFSET -0.5f 
#define GUN_RIGHT_OFFSET -0.2f
#define GUN_RECOIL_STRENGTH 0.5f // Si³a odrzutu
#define GUN_RECOIL_RECOVERY_SPEED 2.0f
#define MUZZLE_FLASH_DURATION 0.05f
#define MUZZLE_LIGHT_FOV 90.0f
#define MUZZLE_LIGHT_RANGE 10.0f
#define MUZZLE_LIGHT_STRENGTH 3.0f
#define GUN_MAX_TURN glm::radians(89.0f)
#define GUN_LOOK_SPEED 0.005f


/*!
 @brief A weapon that can be displayed on screen
*/
class gun : public object {
private:
  light *muzzle_flash_light;
  float muzzle_flash_time;
  glm::vec3 direction; // Direction the gun is pointing
  float recoil;        // Current recoil value
  bool is_firing; 
  float pitch = 15.0f;
  float yaw = 0.0f;
  float roll = 0.0f;
  const float LOOK_SPEED = 0.005f;
  const float MAX_TURN =
      glm::radians(89.0f); // Whether the gun is currently firing
 // wektor kierunku
  glm::vec3 rotation;

public:
  /*!
   @brief Constructs a gun with given shader and position
   @param gun_shader the shader to use for rendering
   @param gun_model the 3D model to use for the gun
   @param position the initial position of the gun
   @param direction the initial direction the gun points
  */
  gun(shader *gun_shader, const model *gun_model,
      glm::vec3 position = glm::vec3(0.0f),
      glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f));

  virtual ~gun();

  /*!
   @brief Gets the direction the gun is pointing
   @return the direction vector
  */
  const glm::vec3 &get_direction() const;
  void update_view_position(const camera *cam);
  void set_muzzle_flash_light(light *flash_light);
  void update_direction();
  void calculate_direction();
  void look_at(glm::vec3 target);


  /*!
   @brief Sets the direction the gun is pointing
   @param new_direction the new direction vector
  */
  void set_direction(const glm::vec3 &new_direction);

  /*!
   @brief Gets the position of the gun
   @return the position vector
  */
  glm::vec3 get_position() const;

  /*!
   @brief Fires the gun
   @return true if the gun fired successfully
  */
  bool fire();

  /*!
   @brief Updates the gun state (recoil, position, etc)
   @param delta_time time since last update
  */
  void update(double delta_time);

  /*!
   @brief Set the position of the gun
   @param xpos The x position
   @param ypos The y position
   @param zpos The z position
  */
  void set_position(double xpos, double ypos, double zpos) ;

  /*!
   @brief Translates the gun
   @param translation the translation to apply
  */
  void translate(glm::vec3 translation) override;

  /*!
   @brief Rotates the gun
   @param xrot rotation around x axis
   @param yrot rotation around y axis
   @param zrot rotation around z axis
  */
  void rotate(glm::vec3 rotation);

  /*!
   @brief Sets the gun's rotation
   @param xrot rotation around x axis
   @param yrot rotation around y axis
   @param zrot rotation around z axis
  */
  void set_rotation(double xrot, double yrot, double zrot) ;
};