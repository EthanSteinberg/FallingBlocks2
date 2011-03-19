#ifndef FIXED_POINT_H_INCLUDED
#define FIXED_POINT_H_INCLUDED

class t_fixed
{
public:
   t_fixed(){}
   t_fixed(const double blah){data = static_cast<int>(blah * (1<<16));}

   operator double(){return static_cast<double>(data)/(1<<16);}
   operator int(){return data;}

private:
   int data;
};


#endif
