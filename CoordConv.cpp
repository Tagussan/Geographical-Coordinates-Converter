#include <cln/cln.h>
#include <iostream>
using namespace std;
using namespace cln;

class CoordConv{
	float_format_t prec;
	cl_F dx,dy,dz,pi;
	cl_F rd;
	cl_F a,a_,f,f_,e2,e2_;
	cl_F xyz[3],llh[3];
public:
	cl_F Tokyo[3],Wgs84[3];
	CoordConv(){
		prec = float_format(16);
		dx="-148.0_16",dy="507.0_16",dz="681.0_16",pi="3.14159265358979323846_16";
		rd = pi/cl_float(180,prec);
		Tokyo[0] = "6377397.155_16";
		Tokyo[1] = "0.003342773179939979_16";
		Tokyo[2] = "0.006674372227347433_16";
		Wgs84[0] = "6378137.0_16";
		Wgs84[1] = "0.003352810671076355_16";
		Wgs84[2] = "0.006694380002756626_16";
	}
	void loadInSystem(cl_F ary[]){
		a = ary[0];f=ary[1];e2=ary[2];
	}
	void loadOutSystem(cl_F ary[]){
		a_ = ary[0];f_=ary[1];e2_=ary[2];
	}
	void llh2xyz(cl_F b,cl_F l,cl_F h,cl_F a,cl_F e2){
		cl_F sb,cb,rn,x,y,z;
		b = b*rd;
		l = l*rd;
		sb = sin(b);
		cb = cos(b);
		rn = a / sqrt(cl_float(1,prec)-e2*sb*sb);

		x=(rn+h)*cb*cos(l);
		y=(rn+h)*cb*sin(l);
		z=rn*(cl_float(1,prec)-e2)+h;
		z = z*sb;
		xyz[0] = x,xyz[1] = y,xyz[2] = z;
		//cout << xyz[0] << " " << xyz[1] << " " << xyz[2] << endl;
	}
	void xyz2llh(cl_F x,cl_F y,cl_F z,cl_F a,cl_F e2){
		cl_F bda,p,t,st,ct,b,l,sb,rn,h;
		bda = sqrt(cl_float(1,prec)-e2);
		p = sqrt(x*x+y*y);
		t = atan(p*bda,z);
		st=sin(t);
		ct=cos(t);
		b=atan(p-e2*a*ct*ct*ct,z+(e2*a/bda)*st*st*st);
		l=atan(x,y);
		sb=sin(b);
		rn=a/sqrt(cl_float(1,prec)-e2*sb*sb);
		h = p/cos(b)-rn;
		//cout << bda << " " << p << " " << t << " " << st << " " << ct << " " << b << " " << l << " " << sb << " " << rn << " " << h << endl;
		llh[0]=b/rd;llh[1]=l/rd;llh[2]=h;
		//cout << llh[0] << " " << llh[1] << " " << llh[2] << endl;
	}
	void convert(cl_F b,cl_F l,cl_F h,cl_F result[]){
		llh2xyz(b,l,h,a,e2);
		xyz2llh(xyz[0]+dx,xyz[1]+dy,xyz[2]+dz,a_,e2_);
		for(int i = 0;i<3;i++)
		result[i] = llh[i];
	}
};

int main(){

	//test
	CoordConv converter;
	converter.loadInSystem(converter.Tokyo);
	converter.loadOutSystem(converter.Wgs84);
	cl_F result[2];
	cl_F inllh[2];
	inllh[0] = "35.678133889_16";
	inllh[1] = "139.769561667_16";
	inllh[2] = "0.0_16";
	//for(int i = 0;i<10;i++)
		converter.convert(inllh[0],inllh[1],inllh[2],result);


	//cout << result[0] << result[1] << result[2];
	return 0;
}
