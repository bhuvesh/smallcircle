#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>


#define sqr(a) (a)*(a)
#define DEBUG if(0)
#define INF 1E+300
using namespace cv;
using namespace std;

Mat back;

int n;

class point
{
	public:
		point(){;}
		point(double xi,double yi)
		{

				x=xi;
				y=yi;

		}
		void show()
		{printf("(%lf,%lf)",x,y);}
		double x,y;	
		
};

class mcircle
{
	public:
		mcircle(){;}
		mcircle(double xi, double yi, double ri)
		{
			r=ri;
			c.x=xi;
			c.y=yi;
		}
		mcircle(point p, double ri)
		{
			r=ri;
			c.x=p.x;
			c.y=p.y;
		}

		mcircle(point *pt1, point *pt2, point *pt3);

		bool IsPerpendicular(point *pt1, point *pt2, point *pt3);
		double CalcCircle(point *pt1, point *pt2, point *pt3);
		void show()
		{
			cout<<"Center : ";c.show();
			cout<<"   Radius : "<<r<<endl;

		}

		point c;
		double r;

};
std::vector<point> v;
mcircle cir;

double length(point a, point b);

bool mcircle::IsPerpendicular(point *pt1, point *pt2, point *pt3)
 
{
	double yDelta_a= pt2->y - pt1->y;
	double xDelta_a= pt2->x - pt1->x;
	double yDelta_b= pt3->y - pt2->y;
	double xDelta_b= pt3->x - pt2->x;
	


	// checking whether the line of the two pts are vertical
	if (fabs(xDelta_a) <= 0.000000001 && fabs(yDelta_b) <= 0.000000001){
	return false;
	}

	if (fabs(yDelta_a) <= 0.0000001)
	{
		return true;
	}
	else if (fabs(yDelta_b) <= 0.0000001)
	{
		return true;
	}
	else if (fabs(xDelta_a)<= 0.000000001)
	{
		return true;
	}
	else if (fabs(xDelta_b)<= 0.000000001)
	{
		return true;
	}
	else return false ;
}

mcircle::mcircle(point *pt1, point *pt2, point *pt3)
{
	r=-1;		// error checking 
	c.x=0;
	c.y=0;
		
	if (!this->IsPerpendicular(pt1, pt2, pt3) )				this->CalcCircle(pt1, pt2, pt3);	
	else if (!this->IsPerpendicular(pt1, pt3, pt2) )		this->CalcCircle(pt1, pt3, pt2);	
	else if (!this->IsPerpendicular(pt2, pt1, pt3) )		this->CalcCircle(pt2, pt1, pt3);	
	else if (!this->IsPerpendicular(pt2, pt3, pt1) )		this->CalcCircle(pt2, pt3, pt1);	
	else if (!this->IsPerpendicular(pt3, pt2, pt1) )		this->CalcCircle(pt3, pt2, pt1);	
	else if (!this->IsPerpendicular(pt3, pt1, pt2) )		this->CalcCircle(pt3, pt1, pt2);	
	else 
	{ 
		r=-1;
		return ;
	}
	

}


double mcircle::CalcCircle(point *pt1, point *pt2, point *pt3)
{
	double yDelta_a= pt2->y - pt1->y;
	double xDelta_a= pt2->x - pt1->x;
	double yDelta_b= pt3->y - pt2->y;
	double xDelta_b= pt3->x - pt2->x;
	
	if (fabs(xDelta_a) <= 0.000000001 && fabs(yDelta_b) <= 0.000000001)
	{
		c.x= 0.5*(pt2->x + pt3->x);
		c.y= 0.5*(pt1->y + pt2->y);
		r= length(c,*pt1);		// calc. radius
		return this->r;
	}
	
	// IsPerpendicular() assure that xDelta(s) are not zero
	double aSlope=yDelta_a/xDelta_a; // 
	double bSlope=yDelta_b/xDelta_b;
	if (fabs(aSlope-bSlope) <= 0.000000001)
	{	// checking whether the given points are colinear. 	
			return -1;
	}

	// calc center
	c.x= (aSlope*bSlope*(pt1->y - pt3->y) + bSlope*(pt1->x + pt2 ->x)	- aSlope*(pt2->x+pt3->x) )/(2* (bSlope-aSlope) );
	c.y = -1*(c.x - (pt1->x+pt2->x)/2)/aSlope +  (pt1->y+pt2->y)/2;
	

	r= length(c,*pt1);		// calc. radius
	return r;
}


double length(point a, point b)
{
return sqrt(sqr(a.x - b.x) + sqr(a.y - b.y));
}

mcircle makeCircle (point a, point b)
{
	mcircle out;
	out.r= length(a,b)/2;
	out.c.x = (a.x + b.x)/2;
	out.c.y	= (a.y + b.y)/2;
	return out;
}

mcircle makeCircle(point a, point b, point c)
{
	mcircle out(&a,&b,&c);
	return out;
}

bool inCircle(point pt, mcircle cir)
{
	if (length(cir.c,pt)<=(cir.r + 0.000000001))
		return true;
	else
		return false;

}



int addPoint(double x, double y)
{
	point input;
	input.x=x;
	input.y=y;
	v.push_back(input);
	n++;

	if(n==0)
		{cir.c.x=v[0].x; cir.c.y=v[0].y; cir.r = 0; return 1;}
	if (n==1)
	{
		cir=makeCircle(v[0],v[1]);
		return 1;		
	}


	
		if (inCircle(v[n],cir))
			{
				DEBUG 
				{
					cout<<"point "<<n<<" has been added.\n";

				}
				return 2;
				
			}
		else
		{
			cir=makeCircle(v[0],v[n]);
			DEBUG
			{
				cout<<"V[0] and v[i] and i being "<<n<<"  \n";
				cir.show();

			}
			for (int j=1;j<n;j++)
			{
				if (inCircle(v[j],cir))
					continue;
				else
				{
					double small=INF; point smallc;
					cir=makeCircle(v[j],v[n]);
					DEBUG
					{
						cout<<"v[j] and v[i] where j: "<<j<<"  and i: "<<n<<endl;
						cir.show();

					}
					int flag=0;
					for (int k=0;k<j;k++)
					{
						if (inCircle(v[k],cir))
							continue;
						else {flag = 1; break;}
					}

					if(!flag)
					{
						small=cir.r;
						smallc= cir.c;
					}


					for (int k=0;k<j;k++)
					{
						flag=0;
						cir=makeCircle(v[k],v[j],v[n]);
						if (cir.r==-1) continue;

						for(int l=0;l<j;l++)
						{
							if (inCircle(v[l],cir))
							continue;
							else {flag = 1; break;}
						}

						if(!flag)
						{
							if (cir.r<small)
							{
								small=cir.r;
								smallc= cir.c;	
							}
							
						}
					}

					cir.r=small;
					cir.c=smallc;
					DEBUG
					{
					cout<<"Small set : ";cir.show();
					}
				}

			}


		}

	return 1;
		

}


void drawPoint(int x, int y)
{
		Point pt =  Point(x, y);
		circle( back, pt, 2, Scalar( 0, 0, 0 ), -1 ,3 );	
}

void drawPoint(int x, int y, Scalar S)
{
		Point pt =  Point(x, y);
		circle( back, pt, 3, S , -1 ,3 );	
}

void drawCircle(double x,double y, double radius)
{
	circle(back, Point(x,y), radius, Scalar(0,0,255), 1, 8);

}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if  ( event == EVENT_LBUTTONDOWN )
	{
		
		int add;
		
		add=addPoint((double)x,(double)y);
		if (add==2)
		{
			drawPoint(x,y);			
		}
		else
		{

			back = Scalar::all(255);

			for(int i=0;i<v.size();i++)
			drawPoint(v[i].x,v[i].y);
			
			drawCircle(cir.c.x, cir.c.y, cir.r);
			drawPoint(cir.c.x,cir.c.y,Scalar(0,0,255));

		}
		add=0;
		cout << "Point number "<<n+1<<" added : (" << x<< ", " << y<< ")" << endl;
		cir.show();
	}
}


int main()
{
	n=-1;
	
	back = imread("back.jpg");
	namedWindow("Circles",CV_WINDOW_AUTOSIZE);
	drawPoint(back.cols/2,back.rows/2,Scalar(255,0,0));
	//drawCircle(250,250,100);
	setMouseCallback("Circles", CallBackFunc, NULL);
	int c=0;
	 //show the image
	while(c!='q')
	{
		imshow("Circles", back);
		c = waitKey(5);

	}
	
	
	return 0;

}