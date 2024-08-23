#include "time_picker.hpp"

#include <SDL.h>
//-----------------------------------------------------------------------------|---------------------------------------|
//                                                                            80
//                                                                            120

TimePicker::TimePicker() { ; }

double TimePicker::getSeconds() { return SDL_GetTicks() / 1000.0; }

double TimePicker::getMiliseconds() { return SDL_GetTicks(); }
//                                                                            80
//                                                                            120
//-----------------------------------------------------------------------------|---------------------------------------|
