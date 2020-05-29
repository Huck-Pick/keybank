/*
 *****************************************************************************
 *    Basado en la libreria para el manejo de Bitmap:                        *
 *       Author: Arash Partow - 2002                                         *
 *       URL: http://partow.net/programming/bitmap/index.html                *
 *                                                                           *
 *****************************************************************************
*/


#ifndef INCLUDE_STK_FILE
#define INCLUDE_STK_FILE

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>


class stk_file{
   public:
      enum read_mode{
         backward_mode = 1
      };
      enum line_plane {
         first_plane  = 0,
         second_plane = 1,
         third_plane   = 2
      };
      struct dat_t{
         unsigned char   first;
         unsigned char second;
         unsigned char  third;
      };
      stk_file(const std::string& filename)
         : file_name_(filename),
         width_          (0),
         height_         (0),
         row_increment_  (0),
         bytes_per_data_(0),
         read_mode_(backward_mode){
            load_stk_file();
         }
      stk_file(const unsigned int width, const unsigned int height)
         : file_name_(""),
         width_ (width ),
         height_(height),
         row_increment_  (0),
         bytes_per_data_(3),
         read_mode_(backward_mode){
            create_stk_file();
         }
      inline bool operator!(){
         return (data_.size()   == 0) || (width_         == 0) || (height_        == 0) || (row_increment_ == 0);
      }
      inline void clear(const unsigned char v = 0x00){
         std::fill(data_.begin(), data_.end(), v);
      }
      inline unsigned char* row(unsigned int row_index) const {
         return const_cast<unsigned char*>(&data_[(row_index * row_increment_)]);
      }
      inline void get_info(const unsigned int x, const unsigned int y, unsigned char& first, 
                           unsigned char& second, unsigned char& third) const{
         const unsigned int y_offset = y * row_increment_;
         const unsigned int x_offset = x * bytes_per_data_;
         const unsigned int offset   = y_offset + x_offset;
         third  = data_[offset + 0];
         second = data_[offset + 1];
         first   = data_[offset + 2];
      }

      template <typename DAT>
      inline void get_info(const unsigned int x, const unsigned int y, DAT& dat) const{
         get_info(x, y, dat.first, dat.second, dat.third);
      }

      inline void set_info(const unsigned int x, const unsigned int y, const unsigned char first,
                           const unsigned char second, const unsigned char third){
         const unsigned int y_offset = y * row_increment_;
         const unsigned int x_offset = x * bytes_per_data_;
         const unsigned int offset   = y_offset + x_offset;
         data_[offset + 0] = third;
         data_[offset + 1] = second;
         data_[offset + 2] = first;
      }

      template <typename DAT>
      inline void set_info(const unsigned int x, const unsigned int y, const DAT& dat){
         set_info(x, y, dat.first, dat.second, dat.third);
      }
      inline unsigned int width() const{
         return width_;
      }
      inline unsigned int height() const{
         return height_;
      }
      inline void setwidth_height(const unsigned int width,
                                 const unsigned int height,
                                 const bool clear = false){
         data_.clear();
         width_  = width;
         height_ = height;

         create_stk_file();

         if (clear){
            std::fill(data_.begin(), data_.end(), static_cast<unsigned char>(0x00));
         }
      }

      void save_stk_file(const std::string& file_name) const{
         std::ofstream stream(file_name.c_str(),std::ios::binary);

         if (!stream){
            std::cerr << "stk_file::save_stk_file(): Error - Could not open file "
                     << file_name << " for writing!" << std::endl;
            return;
         }

         stk_information_header bih;

         bih.width            = width_;
         bih.height           = height_;
         bih.bit_count        = static_cast<unsigned short>(bytes_per_data_ << 3);
         bih.clr_important    = 0;
         bih.clr_used         = 0;
         bih.compression      = 0;
         bih.planes           = 1;
         bih.size             = bih.struct_size();
         bih.x_pels_per_meter = 0;
         bih.y_pels_per_meter = 0;
         bih.size_stk_file       = (((bih.width * bytes_per_data_) + 3) & 0x0000FFFC) * bih.height;

         stk_file_header bfh;

         bfh.type             = 19778;
         bfh.size             = bfh.struct_size() + bih.struct_size() + bih.size_stk_file;
         bfh.reserved1        = 0;
         bfh.reserved2        = 0;
         bfh.off_bits         = bih.struct_size() + bfh.struct_size();

         write_bfh(stream,bfh);
         write_bih(stream,bih);

         unsigned int padding = (4 - ((3 * width_) % 4)) % 4;
         char padding_data[4] = { 0x00, 0x00, 0x00, 0x00 };

         for (unsigned int i = 0; i < height_; ++i){
            const unsigned char* data_ptr = &data_[(row_increment_ * (height_ - i - 1))];

            stream.write(reinterpret_cast<const char*>(data_ptr), sizeof(unsigned char) * bytes_per_data_ * width_);
            stream.write(padding_data,padding);
         }

         stream.close();
      }

      inline const unsigned char* data() const{
         return data_.data();
      }

      inline unsigned char* data(){
         return const_cast<unsigned char*>(data_.data());
      }

      inline unsigned int offset(const line_plane line) const{
         switch (read_mode_){
            case backward_mode : {
                              switch (line)
                              {
                                 case third_plane   : return 2;
                                 case second_plane : return 1;
                                 case first_plane  : return 0;
                                 default          : return std::numeric_limits<unsigned int>::max();
                              }
                           }

            default       : return std::numeric_limits<unsigned int>::max();
         }
      }

   private:

      inline const unsigned char* end() const{
         return data_.data() + data_.size();
      }

      inline unsigned char* end(){
         return const_cast<unsigned char*>(data() + data_.size());
      }

      struct stk_file_header{
         unsigned short type;
         unsigned int   size;
         unsigned short reserved1;
         unsigned short reserved2;
         unsigned int   off_bits;

         unsigned int struct_size() const{
            return   sizeof(type     ) +
                     sizeof(size     ) +
                     sizeof(reserved1) +
                     sizeof(reserved2) +
                     sizeof(off_bits ) ;
         }

         void clear(){
            std::memset(this, 0x00, sizeof(stk_file_header));
         }
      };

      struct stk_information_header{
         unsigned int   size;
         unsigned int   width;
         unsigned int   height;
         unsigned short planes;
         unsigned short bit_count;
         unsigned int   compression;
         unsigned int   size_stk_file;
         unsigned int   x_pels_per_meter;
         unsigned int   y_pels_per_meter;
         unsigned int   clr_used;
         unsigned int   clr_important;

         unsigned int struct_size() const{
            return   sizeof(size                ) +
                     sizeof(width               ) +
                     sizeof(height              ) +
                     sizeof(planes              ) +
                     sizeof(bit_count           ) +
                     sizeof(compression         ) +
                     sizeof(size_stk_file       ) +
                     sizeof(x_pels_per_meter    ) +
                     sizeof(y_pels_per_meter    ) +
                     sizeof(clr_used            ) +
                     sizeof(clr_important       ) ;
         }

         void clear(){
            std::memset(this, 0x00, sizeof(stk_information_header));
         }
      };

      inline bool big_endian() const{
         unsigned int v = 0x01;
         return (1 != reinterpret_cast<char*>(&v)[0]);
      }

      inline unsigned short flip(const unsigned short& v) const{
         return ((v >> 8) | (v << 8));
      }

      inline unsigned int flip(const unsigned int& v) const{
         return (
                  ((v & 0xFF000000) >> 0x18) |
                  ((v & 0x000000FF) << 0x18) |
                  ((v & 0x00FF0000) >> 0x08) |
                  ((v & 0x0000FF00) << 0x08)
               );
      }

      template <typename T>
      inline void read_from_stream(std::ifstream& stream,T& t){
         stream.read(reinterpret_cast<char*>(&t),sizeof(T));
      }

      template <typename T>
      inline void write_to_stream(std::ofstream& stream,const T& t) const{
         stream.write(reinterpret_cast<const char*>(&t),sizeof(T));
      }

      inline void read_bfh(std::ifstream& stream, stk_file_header& bfh){
         read_from_stream(stream,bfh.type     );
         read_from_stream(stream,bfh.size     );
         read_from_stream(stream,bfh.reserved1);
         read_from_stream(stream,bfh.reserved2);
         read_from_stream(stream,bfh.off_bits );

         if (big_endian()){
            bfh.type      = flip(bfh.type     );
            bfh.size      = flip(bfh.size     );
            bfh.reserved1 = flip(bfh.reserved1);
            bfh.reserved2 = flip(bfh.reserved2);
            bfh.off_bits  = flip(bfh.off_bits );
         }
      }

      inline void write_bfh(std::ofstream& stream, const stk_file_header& bfh) const{
         if (big_endian()){
            write_to_stream(stream,flip(bfh.type     ));
            write_to_stream(stream,flip(bfh.size     ));
            write_to_stream(stream,flip(bfh.reserved1));
            write_to_stream(stream,flip(bfh.reserved2));
            write_to_stream(stream,flip(bfh.off_bits ));
         }
         else{
            write_to_stream(stream,bfh.type     );
            write_to_stream(stream,bfh.size     );
            write_to_stream(stream,bfh.reserved1);
            write_to_stream(stream,bfh.reserved2);
            write_to_stream(stream,bfh.off_bits );
         }
      }

      inline void read_bih(std::ifstream& stream,stk_information_header& bih){
         read_from_stream(stream,bih.size            );
         read_from_stream(stream,bih.width           );
         read_from_stream(stream,bih.height          );
         read_from_stream(stream,bih.planes          );
         read_from_stream(stream,bih.bit_count       );
         read_from_stream(stream,bih.compression     );
         read_from_stream(stream,bih.size_stk_file      );
         read_from_stream(stream,bih.x_pels_per_meter);
         read_from_stream(stream,bih.y_pels_per_meter);
         read_from_stream(stream,bih.clr_used        );
         read_from_stream(stream,bih.clr_important   );

         if (big_endian()){
            bih.size          = flip(bih.size               );
            bih.width         = flip(bih.width              );
            bih.height        = flip(bih.height             );
            bih.planes        = flip(bih.planes             );
            bih.bit_count     = flip(bih.bit_count          );
            bih.compression   = flip(bih.compression        );
            bih.size_stk_file    = flip(bih.size_stk_file         );
            bih.x_pels_per_meter = flip(bih.x_pels_per_meter);
            bih.y_pels_per_meter = flip(bih.y_pels_per_meter);
            bih.clr_used      = flip(bih.clr_used           );
            bih.clr_important = flip(bih.clr_important      );
         }
      }

      inline void write_bih(std::ofstream& stream, const stk_information_header& bih) const {
         if (big_endian()){
            write_to_stream(stream,flip(bih.size            ));
            write_to_stream(stream,flip(bih.width           ));
            write_to_stream(stream,flip(bih.height          ));
            write_to_stream(stream,flip(bih.planes          ));
            write_to_stream(stream,flip(bih.bit_count       ));
            write_to_stream(stream,flip(bih.compression     ));
            write_to_stream(stream,flip(bih.size_stk_file      ));
            write_to_stream(stream,flip(bih.x_pels_per_meter));
            write_to_stream(stream,flip(bih.y_pels_per_meter));
            write_to_stream(stream,flip(bih.clr_used        ));
            write_to_stream(stream,flip(bih.clr_important   ));
         }
         else{
            write_to_stream(stream,bih.size            );
            write_to_stream(stream,bih.width           );
            write_to_stream(stream,bih.height          );
            write_to_stream(stream,bih.planes          );
            write_to_stream(stream,bih.bit_count       );
            write_to_stream(stream,bih.compression     );
            write_to_stream(stream,bih.size_stk_file      );
            write_to_stream(stream,bih.x_pels_per_meter);
            write_to_stream(stream,bih.y_pels_per_meter);
            write_to_stream(stream,bih.clr_used        );
            write_to_stream(stream,bih.clr_important   );
         }
      }

      inline std::size_t file_size(const std::string& file_name) const {
         std::ifstream file(file_name.c_str(),std::ios::in | std::ios::binary);
         if (!file) return 0;
         file.seekg (0, std::ios::end);
         return static_cast<std::size_t>(file.tellg());
      }

      void create_stk_file(){
         row_increment_ = width_ * bytes_per_data_;
         data_.resize(height_ * row_increment_);
      }

      void load_stk_file(){
         std::ifstream stream(file_name_.c_str(),std::ios::binary);

         if (!stream){
            std::cerr << "stk_file::load_stk_file() ERROR: stk_file - "
                     << "file " << file_name_ << " not found!" << std::endl;
            return;
         }

         width_  = 0;
         height_ = 0;

         stk_file_header bfh;
         stk_information_header bih;

         bfh.clear();
         bih.clear();

         read_bfh(stream,bfh);
         read_bih(stream,bih);

         if (bfh.type != 19778){
            std::cerr << "stk_file::load_stk_file() ERROR: stk_file - "
                     << "Invalid type value " << bfh.type << " expected 19778." << std::endl;

            bfh.clear();
            bih.clear();

            stream.close();

            return;
         }

         if (bih.bit_count != 24){
            std::cerr << "stk_file::load_stk_file() ERROR: stk_file - "
                     << "Invalid bit depth " << bih.bit_count << " expected 24." << std::endl;

            bfh.clear();
            bih.clear();

            stream.close();

            return;
         }

         if (bih.size != bih.struct_size()){
            std::cerr << "stk_file::load_stk_file() ERROR: stk_file - "
                     << "Invalid BIH size " << bih.size
                     << " expected " << bih.struct_size() << std::endl;

            bfh.clear();
            bih.clear();

            stream.close();

            return;
         }

         width_  = bih.width;
         height_ = bih.height;

         bytes_per_data_ = bih.bit_count >> 3;

         unsigned int padding = (4 - ((3 * width_) % 4)) % 4;
         char padding_data[4] = { 0x00, 0x00, 0x00, 0x00 };

         std::size_t stk_file_size = file_size(file_name_);

         std::size_t stk_logical_size = (height_ * width_ * bytes_per_data_) +
                                          (height_ * padding)                   +
                                          bih.struct_size()                    +
                                          bfh.struct_size()                    ;

         if (stk_file_size != stk_logical_size){
            std::cerr << "stk_file::load_stk_file() ERROR: stk_file - "
                     << "Mismatch between logical and physical sizes of stk file. "
                     << "Logical: "  << stk_logical_size << " "
                     << "Physical: " << stk_file_size    << std::endl;

            bfh.clear();
            bih.clear();

            stream.close();

            return;
         }

         create_stk_file();

         for (unsigned int i = 0; i < height_; ++i){
            unsigned char* data_ptr = row(height_ - i - 1); // read in inverted row order

            stream.read(reinterpret_cast<char*>(data_ptr), sizeof(char) * bytes_per_data_ * width_);
            stream.read(padding_data,padding);
         }
      }

      template <typename T>
      inline T clamp(const T& v, const T& lower_range, const T& upper_range) const{
         if (v < lower_range)
            return lower_range;
         else if (v >  upper_range)
            return upper_range;
         else
            return v;
      }

      std::string  file_name_;
      unsigned int width_;
      unsigned int height_;
      unsigned int row_increment_;
      unsigned int bytes_per_data_;
      read_mode read_mode_;
      std::vector<unsigned char> data_;
};
typedef stk_file::dat_t dat_t;
#endif