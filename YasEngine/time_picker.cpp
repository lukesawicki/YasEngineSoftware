#include "time_picker.hpp"

#include <SDL.h>
//-----------------------------------------------------------------------------|---------------------------------------|
//                                                                            80
//                                                                            120

TimePicker::TimePicker() { ; }

double TimePicker::GetSeconds()
{
  return SDL_GetTicks() / 1000.0;
}

double TimePicker::GetMiliseconds()
{
  return static_cast<double>(SDL_GetTicks());
}  
//                                                                            80
//                                                                            120
//-----------------------------------------------------------------------------|---------------------------------------|
