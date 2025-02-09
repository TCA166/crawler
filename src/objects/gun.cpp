#include "gun.hpp"
#include "../engine/engine.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


gun::gun(shader *gun_shader, const model *gun_model, glm::vec3 position,
         glm::vec3 direction)
    : object( gun_model, position.x, position.y, position.z),
      direction(direction), recoil(0.0f), is_firing(false) {
  // Initialize gun with proper scale
  object::set_scale(GUN_SCALE);
}

gun::~gun() {}

const glm::vec3 &gun::get_direction() const { return direction; }

void gun::set_direction(const glm::vec3 &new_direction) {

  direction = glm::normalize(new_direction);

  if (recoil > 0.0f) {
    glm::vec3 current_pos = get_position();
    glm::vec3 recoil_offset =
        -direction * recoil;
    set_position(current_pos.x + recoil_offset.x,
                 current_pos.y + recoil_offset.y,
                 current_pos.z + recoil_offset.z);
  }
}

bool gun::fire() {
  if (!is_firing) {
    is_firing = true;
    recoil = GUN_RECOIL_STRENGTH;
    muzzle_flash_time = MUZZLE_FLASH_DURATION;

    if (muzzle_flash_light) {
  
      muzzle_flash_light->set_color(glm::vec3(MUZZLE_LIGHT_STRENGTH));


      glm::vec3 flash_position = get_position() + direction * 2.0f;
      muzzle_flash_light->set_position(flash_position);
      muzzle_flash_light->look_at(get_position() + direction +2.0f);

      // Ustaw kierunek œwiat³a
      //muzzle_flash_light->set_direction(get_position() + direction * 10.0f);
    }
    return true;
  }
  return false;
}

void gun::update(double delta_time) {
  if (recoil > 0.0f) {
    recoil =
        std::max(0.0f, recoil - GUN_RECOIL_RECOVERY_SPEED * (float)delta_time);
    if (recoil <= 0.0f) {
      is_firing = false;
    }
  }

  if (muzzle_flash_time > 0.0f) {
    muzzle_flash_time -= delta_time;
    if (muzzle_flash_time <= 0.0f) {
      if (muzzle_flash_light) {
        muzzle_flash_light->set_color(glm::vec3(0.0f));
      }
    }
  }
}


void gun::set_muzzle_flash_light(light *flash_light) {
  muzzle_flash_light = flash_light;
  if (muzzle_flash_light) {
    muzzle_flash_light->set_color(glm::vec3(0.0f));
  }
}


glm::vec3 gun::get_position() const {
  return glm::vec3(position);
}

void gun::set_rotation(double xrot, double yrot, double zrot) {
  object::set_rotation(glm::vec3(xrot, yrot, zrot));
  direction = glm::vec3(0.0f, 0.0f, -1.0f); 
  rotate(glm::vec3(xrot, yrot, zrot));
}


void gun::set_position(double xpos, double ypos, double zpos) {
  object::set_position(glm::vec3(xpos, ypos, zpos));
}

void gun::translate(glm::vec3 translation) { object::translate(translation); }


void gun::rotate(glm::vec3 rotation) {
  object::rotate(rotation);
 
}

void gun::update_direction() {
  direction.x = cos(yaw) * cos(pitch);
  direction.y = sin(pitch);
  direction.z = sin(yaw) * cos(pitch);
  direction = glm::normalize(direction);

  if (recoil > 0.0f) {
    glm::vec3 current_pos = get_position();
    glm::vec3 recoil_offset = -direction * recoil;
    set_position(current_pos.x + recoil_offset.x,
                 current_pos.y + recoil_offset.y,
                 current_pos.z + recoil_offset.z);
  }
}

void gun::look_at(glm::vec3 target) {
  // Oblicz znormalizowany wektor kierunku od pozycji broni do celu
  glm::vec3 new_direction = glm::normalize(target - get_position());

  // Uaktualnij aktualny direction broni
  direction = new_direction;

  // Ustaw rotacjê obiektu broni
/*  object::set_rotation(glm::vec3(0.0f)); */// Reset rotacji

  //// Oblicz pozycjê broni z uwzglêdnieniem odrzutu
  //glm::vec3 current_pos = get_position();
  //if (recoil > 0.0f) {
  //  current_pos += -direction * recoil; // Dodaj przesuniêcie od odrzutu
  //}

  //// Aktualizuj pozycjê broni
  //set_position(current_pos);

  // Jeœli aktywny jest b³ysk wylotowy, zaktualizuj jego pozycjê i kierunek

}