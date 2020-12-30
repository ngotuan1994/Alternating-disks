///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// TODO
#include <functional>
#include <iostream>

enum disk_color { DISK_DARK, DISK_LIGHT};

class disk_state {
private:
  std::vector<disk_color> _colors;

public:
  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_DARK) {

    assert(light_count > 0);

    for (size_t i = 0; i < _colors.size(); i += 2) {
      _colors[i] = DISK_LIGHT;
    }
  }

  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_initialized() const {

    for(unsigned int i = 0; i < _colors.size(); i++)
    {
      if(i % 2 == 0)
      {
        if(_colors[i] == DISK_DARK)
        {
          return false;
        }
      }
      else
      {
        if(_colors[i] == DISK_LIGHT)
        {
          return false;
        }
      }
    }
    return true;
  }

  // Return true when this disk_state is fully sorted, with all dark disks
  // on the left (low indices) and all light disks on the right (high
  // indices).
  bool is_sorted() const {
    for(unsigned int i = 0; i < _colors.size()/2; i++)
    {
      if( get(i) == DISK_LIGHT)
      {
        return false;
      }
      if ( get(_colors.size() - 1 - i) == DISK_DARK)
      {
        return false;
      }
    }
    return true;
    }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the alternate algorithm.
sorted_disks sort_alternate(const disk_state& before) {
  /*
  Note:
  Use unsigned int for loop because of index of vector >=0
  get Larger range for loop count variable
  */

  disk_state tmp = before;
  unsigned int count = 0;
  for(unsigned int i = 0; i < tmp.total_count(); i++)
  {
    // j= 1 + i  The +i to avoid check the element we already put on the left side
    // previous move
    for(unsigned int j = 1 + i; j < tmp.total_count(); j++)
    {
      if(tmp.get(j - 1) == DISK_LIGHT && tmp.get(j) == DISK_DARK)
      {
        tmp.swap(j - 1);
        count++;
      }
    }
  }
  return sorted_disks(tmp, count);
}


// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
  /*
  Note:
  Use unsigned int for loop because of index of vector >=0
  get Larger range for loop count variable
  */
  disk_state tmp = before;
  unsigned int count = 0;
  // Because we run left-> right and right to left in one time
  // that why we only need n/2 repeat times to sort with n=size(Disks)
  for(unsigned int i = 0; i < tmp.total_count() / 2; i++)
  {
    // j= 1 + i  The +i to avoid check the element we already put on the left side
    // previous move
    for(unsigned int j = 1 + i; j < tmp.total_count(); j++)
    {
      if(tmp.get(j - 1) == DISK_LIGHT && tmp.get(j) == DISK_DARK)
      {
        tmp.swap(j - 1);
        count++;
      }
    }
    // k=size(disks)-1-i  The -i to avoid check the element we alreaedy put on
    //the right side previous move

    for(unsigned int k = tmp.total_count() - 1 - i; k > 0; k--)
    {
      if(tmp.get(k) == DISK_DARK && tmp.get(k) == DISK_LIGHT)
      {
        tmp.swap(k - 1);
        count++;
      }
    }
  }
  return sorted_disks(tmp, count);
}
