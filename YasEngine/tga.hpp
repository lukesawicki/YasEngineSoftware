#include<cstddef>

class Tga {
  
  public:
    // Tga header {
    const long dataPositions[] = {0, 1, 2, 3, 5, 7, 8, 10, 12, 14, 16, 17, 18, -1, -1, -1, -1 }; // last value i end of image_data_
    std::byte id_length_; // byte index = 0
    std::byte color_map_type_;  // byte index = 1
    std::byte image_type_;  // byte index = 2
      // Color map specification {
      unsigned short first_entry_index_;  // byte index = 3
      unsigned short color_map_length_; // byte index = 5
      std::byte color_map_entry_size_; // byte index = 7
      // end of color map specification
      
      // Image specification
      unsigned short x_origin_; // byte index = 8
      unsigned short y_origin_;  // byte index = 10
      unsigned short image_width_;  // byte index = 12
      unsigned short image_height_;  // byte index = 14
      std::byte pixel_depth_;        // byte index = 16
      std::byte image_descriptor_; // byte index = 17 | bits 3–0 give the alpha channel depth, bits 5–4 give pixel ordering
      // Bit 4 of the image descriptor byte indicates right - to -
      // left pixel ordering if set.Bit 5 indicates an ordering of top - to -
      // bottom.Otherwise, pixels are stored in bottom - to - top, left - to - right order.
      // end image specification
    //} end of tga header;

    // Image and color map data
    std::byte* image_id_; // byte index = 18 | length of memory for this value is in id_length_
      std::byte* color_map_; // byte index = is index of image_id_ + value from id_length_

    std::byte* image_data_; // position calculated 

  };