#ifndef DATA_H
#define DATA_H


typedef struct {
    float R;
    float G;
    float B;
	float get(int i) { 
		if(0 <= i && i <= 3) return (i == 0) ?  R : (i == 1) ? G : B; 
		else 0;
	}
} Color;


typedef struct {
    float dim[3];
    public:
      float x = dim[0], y = dim[1], z = dim[2];
} Point;

#endif