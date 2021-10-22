#ifndef UTILS_H
#define UTILS_H

#include <cinolib/meshes/meshes.h>

struct vert_compare{
    bool operator()(const cinolib::vec3d& a, const cinolib::vec3d& b) const {

       double eps = 1e-6;
       if(a.x()-b.x() < 0.0 && abs(a.x()-b.x()) > eps){
           return true;
       }
       else if(abs(a.x()-b.x()) < eps){
           if(a.y()-b.y() < 0.0 && abs(a.y()-b.y()) > eps){
               return true;
           }
           else if(abs(a.y()-b.y()) < eps){
               if(a.z()-b.z() < 0.0 && abs(a.z()-b.z()) > eps){
                   return true;
               }
           }
       }

       return false;
    }
};


inline bool eps_eq(const cinolib::vec3d &a, const cinolib::vec3d &b, double eps = 1e-6){
    return (a.dist(b) <= eps);
}




#endif // UTILS_H