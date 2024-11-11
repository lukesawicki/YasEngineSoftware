#include "time_picker.hpp"
#include <SDL.h>

TimePicker::TimePicker()
{
  ;
}

double TimePicker::getSeconds()
{
  return SDL_GetTicks() / 1000.0;
}

double TimePicker::getMiliseconds()
{
  return SDL_GetTicks();
}
