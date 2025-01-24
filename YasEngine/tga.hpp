#include<cstddef>

class Tga {
  
  public:
    // Tga header {
    std::byte id_length_;
    std::byte color_map_type_;
    std::byte image_type_;
      // Color map specification {
      unsigned short first_entry_index_;
      unsigned short color_map_length_;
      std::byte color_map_entry_size_;
      // end of color map specification
      
      // Image specification
      unsigned short x_origin_;
      unsigned short y_origin_;
      unsigned short image_width_;
      unsigned short image_height_;
      std::byte pixel_depth_;
      std::byte image_descriptor_; // bits 3–0 give the alpha channel depth, bits 5–4 give pixel ordering
      // Bit 4 of the image descriptor byte indicates right - to -
      // left pixel ordering if set.Bit 5 indicates an ordering of top - to -
      // bottom.Otherwise, pixels are stored in bottom - to - top, left - to - right order.
      // end image specification
    //} end of tga header;

    // Image and color map data
    std::byte* image_id_;
    std::byte* color_map_;
    std::byte* image_data_;

  };