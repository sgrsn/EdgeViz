#ifndef _CONSOLE_VIZ_HPP_
#define _CONSOLE_VIZ_HPP_

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#include <vector>
#include <string>
#include <ArduinoEigen.h>

namespace EdgeViz
{

class ConsoleViz
{

 public:
  ConsoleViz(uint16_t col_size, uint16_t row_size, double console_aspect_ratio=2.5) : 
    n_cols_(col_size), 
    n_rows_(row_size), 
    console_aspect_ratio_(console_aspect_ratio)
  {
    rows_.resize(n_rows_);
  }
  virtual ~ConsoleViz() = default;

  void line(const std::vector<int16_t>& p1, const std::vector<int16_t>& p2, const char* color, const uint16_t& thickness=1);
  std::vector<int16_t> rotate(const std::vector<int16_t>& pt, const std::vector<int16_t>& origin, float angle);
  void rectangle(const std::vector<int16_t>& p1, const std::vector<int16_t>& p2, const char* color, const uint16_t& thickness=1);
  void rectangle(const std::vector<int16_t>& p1, const std::vector<int16_t>& p2, float angle, const char* color, const uint16_t& thickness=1);
  void circle(const std::vector<int16_t>& center, uint8_t radius, const char* color, const uint16_t& thickness=1);

  void box(const std::vector<int16_t>& p, const std::vector<int16_t>& size, Eigen::Matrix3d rot, const char* color, const uint16_t& thickness=1);

  void fill(const std::vector<std::vector<int16_t>>& points, char* color);
  void clear();
  void imshow(bool flip_y=false);

 private:
  std::vector<std::vector<std::pair<int16_t, char*>>> rows_;
  uint16_t n_rows_;
  uint16_t n_cols_;
  double console_aspect_ratio_;
};

} // namespace EdgeViz

#endif // _CONSOLE_VIZ_HPP_