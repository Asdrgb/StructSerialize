#pragma once
#include<vector>
#include "Serializer.h"




DEFINE_STRUCT(Point,
	(int) x,
	(double) y,
	(bool) flag,
	(std::string) str,
	(QString) qstr
);


//DEFINE_STRUCT(Rect,
//	(Point)p1,
//	(Point)p2,
//	(uint32_t)color
//);


DEFINE_STRUCT(ContainerNest,
	(std::vector<QString>) q,
	(std::vector<int>) x,
	(std::vector<double>) y,
	(std::vector<std::string>) z,
	(std::vector<std::vector<QString>>) v
	
);

DEFINE_STRUCT(Nested,
	(Point) x,
	(ContainerNest) y,
	(std::string) name1,
	(QString) name2,
	(int) num
);


