#include "console_viz.hpp"
#include <algorithm>
#include <vector>
#include <tuple>
#include <string>
#include <iomanip>
#include <Arduino.h>
#include <ArduinoEigen.h>

namespace EdgeViz
{

void ConsoleViz::line(const std::vector<int16_t>& p1, const std::vector<int16_t>& p2, const char* color, const uint16_t& thickness)
{
  // p1=(x1, y1), p2=(x2, y2)
  // y = (y2-y1)/(x2-x1) * (x-x1) + y1

  // 振り潰す座標のvector 
  std::vector<std::vector<int16_t>> fill_points;//(n_point, std::vector<uint16_t>(2, 0));

  // x軸で走査して座標を追加
  std::vector<int16_t> start_point;
  std::vector<int16_t> end_point;

  if(p1[0] == p2[0])
  {
  }
  else
  {
    if(p1[0] < p2[0])
    {
      start_point = p1;
      end_point = p2;
    }
    else
    {
      start_point = p2;
      end_point = p1;
    }
    for(int16_t x=start_point[0]; x<=end_point[0]; x++)
    {
      std::vector<int16_t> point{
        x, 
        (int16_t)((double)(p2[1]-p1[1])/(double)(p2[0]-p1[0]) * (double)( x - p1[0]) + (double)p1[1] + 0.5)
      };
      fill_points.emplace_back(point);
    }
  }

  // y軸で走査して座標を追加
  // xとyとも行うことで線がスカスカになることを防ぐ
  if(p1[1] == p2[1])
  {
  }
  else
  {
    if(p1[1] < p2[1])
    {
      start_point = p1;
      end_point = p2;
    }
    else
    {
      start_point = p2;
      end_point = p1;
    }
    for(int16_t y=start_point[1]; y<=end_point[1]; y++)
    {
      std::vector<int16_t> point{
        (int16_t)((double)(p2[0]-p1[0])/(double)(p2[1]-p1[1]) * (double)( y - p1[1]) + (double)p1[0] + 0.5), 
        y
      };
      fill_points.emplace_back(point);
    }
  }

  fill(fill_points, color);
}

void ConsoleViz::rectangle(const std::vector<int16_t>& p1, const std::vector<int16_t>& p2, const char* color, const uint16_t& thickness)
{
  std::vector<int16_t> pt1{p1[0], p1[1]};
  std::vector<int16_t> pt2{p2[0], p1[1]};
  std::vector<int16_t> pt3{p2[0], p2[1]};
  std::vector<int16_t> pt4{p1[0], p2[1]};
  
  line(pt1, pt2, color, thickness);
  line(pt2, pt3, color, thickness);
  line(pt3, pt4, color, thickness);
  line(pt4, pt1, color, thickness);
}

std::vector<int16_t> ConsoleViz::rotate(const std::vector<int16_t>& pt, const std::vector<int16_t>& origin, float angle)
{
  //回転行列の作成
  Eigen::Matrix2f rot;
  rot = Eigen::Rotation2Df(angle);
  Eigen::VectorXf ptv(2);
  ptv << pt[0]-origin[0], pt[1]-origin[1];
  ptv = rot * ptv;
  return std::vector<int16_t>{(int16_t)ptv[0]+origin[0], (int16_t)ptv[1]+origin[1]};
}

void ConsoleViz::rectangle(const std::vector<int16_t>& p1, const std::vector<int16_t>& p2, float angle, const char* color, const uint16_t& thickness)
{
  std::vector<int16_t> v1 = rotate(std::vector<int16_t>{p1[0], p1[1]}, p1, angle);
  std::vector<int16_t> v2 = rotate(std::vector<int16_t>{p2[0], p1[1]}, p1, angle);
  std::vector<int16_t> v3 = rotate(std::vector<int16_t>{p2[0], p2[1]}, p1, angle);
  std::vector<int16_t> v4 = rotate(std::vector<int16_t>{p1[0], p2[1]}, p1, angle);
  line(v1, v2, color, thickness);
  line(v2, v3, color, thickness);
  line(v3, v4, color, thickness);
  line(v4, v1, color, thickness);
}

void ConsoleViz::circle(const std::vector<int16_t>& center, uint8_t radius, const char* color, const uint16_t& thickness)
{
  // 塗り潰す座標のvector 
  std::vector<std::vector<int16_t>> fill_points;//(n_point, std::vector<uint16_t>(2, 0));
  for(int deg=0; deg<360; deg++)
  {
    std::vector<int16_t> point{
      (double)center[0] + (double)radius*cos((double)deg*DEG_TO_RAD), 
      (double)center[1] + (double)radius*sin((double)deg*DEG_TO_RAD)
    };
    fill_points.emplace_back(point);
  }
  fill(fill_points, color);
}

// 座標と色を渡すと塗る関数
void ConsoleViz::fill(const std::vector<std::vector<int16_t>>& points, char* color)
{
  // y昇順にソート
  //sort(points.begin(), points.end(),[](const std::vector<uint16_t> &alpha,const std::vector<uint16_t> &beta){return alpha[1] < beta[1];});
  
  // yが同じ値でグループ化
  for(auto pt : points)
  {
    if(pt[1] >= rows_.size() || pt[1] < 0) continue;
    if(pt[0] >= n_cols_ || pt[0] < 0) continue;
    std::pair<int16_t, char*> p = std::make_pair(pt[0], color);
    rows_[pt[1]].emplace_back( p ); // y行目に(x座標, 色)のpairを追加
  }
  for(int i = 0; i < n_rows_; i++)
  {
    // x昇順にソート 重複した場合はcolorでソートされる
    sort(rows_[i].begin(), rows_[i].end());
    // 重複要素を削除
    rows_[i].erase(std::unique(rows_[i].begin(), rows_[i].end()), rows_[i].end());
  }
}

void ConsoleViz::clear()
{
  rows_.clear();
  rows_.resize(n_rows_);
}

void ConsoleViz::imshow()
{
  std::stringstream ss;
  ss.str("");
  ss << GREEN;
  for(int i = 0; i <= n_cols_; i++) ss << "#####";
  Serial.println(ss.str().c_str());
  Serial.println(ss.str().c_str());
  for(auto row : rows_)
  {
    ss.str("");
    ss << GREEN << "#####";
    int last_x_value = 0;
    for(auto pair : row)
    {
      int n_brank = pair.first-last_x_value-1;
      for(int i = 0; i < n_brank; i++) ss << "     ";
      ss << pair.second; // color
      if(n_brank < 0){} 
      else 
        ss << "▧▧▧▧▧";
      last_x_value = pair.first;
    }
    int n_brank = n_cols_-last_x_value-1;
    for(int i = 0; i < n_brank; i++) ss << "     ";
    ss << GREEN << "#####";
    Serial.println(ss.str().c_str());
    Serial.println(ss.str().c_str());
  }
  ss.str("");
  ss << GREEN;
  for(int i = 0; i <= n_cols_; i++) ss << "#####";
  Serial.println(ss.str().c_str());
  Serial.println(ss.str().c_str());
}

} // namespace EdgeViz