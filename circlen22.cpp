#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>

#define sqr(a) (a)*(a)
#define DEBUG if(0)
#define INF 1E+300
using namespace std;


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

class circle
{
	public:
		circle(){;}
		circle(double xi, double yi, double ri)
		{
			r=ri;
			c.x=xi;
			c.y=yi;
		}
		circle(point p, double ri)
		{
			r=ri;
			c.x=p.x;
			c.y=p.y;
		}

		circle(point *pt1, point *pt2, point *pt3);

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
double length(point a, point b);

bool circle::IsPerpendicular(point *pt1, point *pt2, point *pt3)
 
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

circle::circle(point *pt1, point *pt2, point *pt3)
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


double circle::CalcCircle(point *pt1, point *pt2, point *pt3)
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

circle makeCircle (point a, point b)
{
	circle out;
	out.r= length(a,b)/2;
	out.c.x = (a.x + b.x)/2;
	out.c.y	= (a.y + b.y)/2;
	return out;
}

circle makeCircle(point a, point b, point c)
{
	circle out(&a,&b,&c);
	return out;
}

bool inCircle(point pt, circle cir)
{
	if (length(cir.c,pt)<=(cir.r + 0.000000001))
		return true;
	else
		return false;

}

std::vector<point> v;
int main()
{
	int n;
	//cout<<"Enter number of points: (at least 2) \n";
	cin>>n;
	//cout<<"Enter the coordinates of all the points: \n";
	point input;
	for (int i=0;i<n;i++)
	{
		cin>>input.x>>input.y;
		v.push_back(input);
	}
	cout<<endl;
	DEBUG for (int i=0;i<n;i++)
	{
		v[i].show();
		cout<<endl;
	}

	circle cir=makeCircle(v[0],v[1]);
	DEBUG
	{
		cout <<"Circle with first two made. \n";
		cir.show();

	}
	for(int i=2;i<n;i++)
	{
		if (inCircle(v[i],cir))
			{
				DEBUG 
				{
					cout<<"point "<<i<<" has been added.\n";

				}
				continue;
				
			}
		else
		{
			cir=makeCircle(v[0],v[i]);
			DEBUG
			{
				cout<<"V[0] and v[i] and i being "<<i<<"  \n";
				cir.show();

			}
			for (int j=1;j<i;j++)
			{
				if (inCircle(v[j],cir))
					continue;
				else
				{
					double small=INF; point smallc;
					cir=makeCircle(v[j],v[i]);
					DEBUG
					{
						cout<<"v[j] and v[i] where j: "<<j<<"  and i: "<<i<<endl;
						cir.show();

					}
					int flag1=0;
					for (int k=0;k<j;k++)
					{
						if (inCircle(v[k],cir))
							continue;
						else {flag1 = 1; break;}
					}

					if(!flag1)
					{
						small=cir.r;
						smallc= cir.c;
					}

					if (flag1)
					for (int k=0;k<j;k++)
					{
						int flag=0;
						cir=makeCircle(v[k],v[j],v[i]);
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

	}


	cout<<"The circle has been found.\n The center : "; cir.c.show();
	cout<<"   Radius: "<<cir.r<<endl;
}