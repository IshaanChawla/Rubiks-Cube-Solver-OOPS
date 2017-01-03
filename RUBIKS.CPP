#include<dos.h>
#include<math.h>
#include<conio.h>
#include<string.h>
#include<iostream.h>
#include<graphics.h>
union REGS i,o;
struct mospos
{
	int x,y,button;
}m;
struct Adjface
{
	int num;
	struct Adjface *next,*prev;
};
struct dblptr
{
	Adjface *head1,*head2;
};
class Square
{
	private:
		int color,pos,mvb;
	public:
		Square();
		Square(int);
		void setcolor(int clr);
		int getcolor();
};
Square :: Square()
{
	color=0;
}
Square :: Square (int p)
{
	pos=p;
	if(pos%9==4)
		mvb=0;
	else
		mvb=1;
	color=0;
}
void Square :: setcolor(int clr)
{
	color=clr;
}
int Square :: getcolor()
{
	return color;
}
class Row
{
	private:
		Square ls,cs,rs;
	public:
		Row()
		{
		}
		Row(int *x,int y):ls(x[y]),cs(x[y+1]),rs(x[y+2])
		{
		}
		Square& getsquare(int);
};
Square& Row :: getsquare(int k)
{
	switch(k%3)
	{
		case 0:
			return ls;
		case 1:
			return cs;
		case 2:
			return rs;
	}
}
class Face
{
	private:
		char name[7];
		int face_no,face_color;
		Row tr,cr,br;
		static dblptr h05,h13,h24;
	public:
		Face(char*,int);
		Face(char*,int,int*,int);
		Row& getrow(int);
		void setface_color(int);
		int getface_color();
		char* getfacename();
		int getface_no();
		static void createadjface();
		static dblptr& getadjface(int);
};
dblptr Face :: h05;
dblptr Face :: h13;
dblptr Face :: h24;
Face :: Face(char* name,int num)
{
	strcpy(this->name,name);
	face_no=num;
	face_color=0;
}
Face :: Face(char* name,int num,int *x,int y):tr(x,y),cr(x,y+3),br(x,y+6)
{
	strcpy(this->name,name);
	face_no=num;
	face_color=x[num*9+4];
}
Row& Face :: getrow(int j)
{
	switch(j%9)
	{
		case 0:
		case 1:
		case 2:
			return tr;
		case 3:
		case 4:
		case 5:
			return cr;
		case 6:
		case 7:
		case 8:
			return br;
	}
}
char* Face :: getfacename()
{
	return name;
}
int Face :: getface_no()
{
	return face_no;
}
void Face :: setface_color(int x)
{
	face_color=x;
}
int Face :: getface_color()
{
	return face_color;
}
void Face :: createadjface()
{
	Adjface* temp;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
		{
			temp=new Adjface();
			temp->prev=NULL;
			temp->next=NULL;
			if(i==0)
			{
				switch(j)
				{
					case 0:
						temp->num=2;
						break;
					case 1:
						temp->num=1;
						break;
					case 2:
						temp->num=4;
						break;
					case 3:
						temp->num=3;
						break;
				}
				if(h05.head1==NULL)
					h05.head1=temp;
				else
					h05.head2->next=temp;
				temp->next=h05.head1;
				temp->prev=h05.head2;
				h05.head2=temp;
				h05.head1->prev=h05.head2;
			}
			if(i==1)
			{
				switch(j)
				{
					case 0:
						temp->num=5;
						break;
					case 1:
						temp->num=2;
						break;
					case 2:
						temp->num=0;
						break;
					case 3:
						temp->num=4;
						break;
				}
				if(h13.head1==NULL)
					h13.head1=temp;
				else
					h13.head2->next=temp;
				temp->next=h13.head1;
				temp->prev=h13.head2;
				h13.head2=temp;
				h13.head1->prev=h13.head2;
			}
			if(i==2)
			{
				switch(j)
				{
					case 0:
						temp->num=5;
						break;
					case 1:
						temp->num=1;
						break;
					case 2:
						temp->num=0;
						break;
					case 3:
						temp->num=3;
						break;
				}
				if(h24.head1==NULL)
					h24.head1=temp;
				else
					h24.head2->next=temp;
				temp->next=h24.head1;
				temp->prev=h24.head2;
				h24.head2=temp;
				h24.head1->prev=h24.head2;
			}
		}
	}
}
dblptr& Face :: getadjface(int face_num)
{
	if(face_num==0 || face_num==5)
		return h05;
	else if(face_num==1 || face_num==3)
		return h13;
	else
		return h24;
}
class Cube
{
	private:
		Face top,left,front,right,back,bottom;
		int count;
	public:
		Cube():top("Top",0),left("Left",1),front("Front",2),right("Right",3),back("Back",4),bottom("Bottom",5)
		{
			count=0;
		}
		Cube(int *x):top("Top",0,x,0),left("Left",1,x,9),front("Front",2,x,18),right("Right",3,x,27),back("Back",4,x,36),bottom("Bottom",5,x,45)
		{
			count=0;
		}
		Face& getface(int);
		void operator++();
		int getcount();
		int operator==(Cube*);
};
Face& Cube :: getface(int i)
{
	if(i>=0 && i<9)
		return top;
	else if(i>=9 && i<18)
		return left;
	else if(i>=18 && i<27)
		return front;
	else if(i>=27 && i<36)
		return right;
	else if(i>=36 && i<45)
		return back;
	else
		return bottom;
}
void Cube :: operator++()
{
	count++;
}
int Cube :: getcount()
{
	return count;
}
int Cube :: operator==(Cube *x)
{
	for(int i=0;i<54;i++)
		if(getface(i).getrow(i).getsquare(i).getcolor()!=0)
			if(getface(i).getrow(i).getsquare(i).getcolor()!=x->getface(i).getrow(i).getsquare(i).getcolor())
				return 0;
	return 1;
}
class Moves
{
	private:
		char initials;
		int move_no,clock;
		static Cube* rub;
		static int corner[4],edge[4],corner1[4],edge1[4],corner2[4];
	public:
		Moves();
		Moves(int,int);
		void setinitials();
		char getinitials();
		void setmove(int,int);
		static void addcube(Cube*);
		void changestate();
		int getmove();
		int getclock();
		static Cube* getcube();
		static void selectsquares(int);
		static int* getedge1();
		static int* getcorner1();
		static int* getcorner2();
};
Cube* Moves :: rub=NULL;
int Moves :: corner[4]={0,2,8,6};
int Moves :: edge[4]={1,5,7,3};
int Moves :: corner1[4];
int Moves :: edge1[4];
int Moves :: corner2[4];
Moves :: Moves()
{
	move_no=-1;
	clock=-1;
}
Moves :: Moves(int m,int c)
{
	move_no=m;
	clock=c;
	setinitials();
}
void Moves :: setinitials()
{
	switch(move_no)
	{
		case 0:
			initials='T';
			break;
		case 1:
			initials='L';
			break;
		case 2:
			initials='F';
			break;
		case 3:
			initials='R';
			break;
		case 4:
			initials='B';
			break;
		case 5:
			initials='D';
			break;
	}
}
char Moves :: getinitials()
{
	return initials;
}
void Moves :: setmove(int m,int c)
{
	move_no=m;
	clock=c;
	setinitials();
}
void Moves :: addcube(Cube *x)
{
	rub=x;
}
void Moves :: selectsquares(int move)
{
	int i,x;
	switch(move)
	{
		case 0:
			for(i=0;i<4;i++)
			{
				corner1[i]=0;
				corner2[i]=2;
				edge1[i]=1;
			}
			break;
		case 1:
			corner1[0]=corner1[1]=corner1[2]=6;
			corner1[3]=2;
			corner2[0]=corner2[1]=corner2[2]=0;
			corner2[3]=8;
			edge1[0]=edge1[1]=edge1[2]=3;
			edge1[3]=5;
			break;
		case 2:
			corner1[0]=0;
			corner1[1]=2;
			corner1[2]=8;
			corner1[3]=6;
			edge1[0]=1;
			edge1[1]=5;
			edge1[2]=7;
			edge1[3]=3;
			corner2[0]=2;
			corner2[1]=8;
			corner2[2]=6;
			corner2[3]=0;
			break;
		case 3:
			corner1[0]=corner1[1]=corner1[2]=2;
			corner1[3]=6;
			corner2[0]=corner2[1]=corner2[2]=8;
			corner2[3]=0;
			edge1[0]=edge1[1]=edge1[2]=5;
			edge1[3]=3;
			break;
		case 4:
			corner1[0]=8;
			corner1[1]=6;
			corner1[2]=0;
			corner1[3]=2;
			edge1[0]=7;
			edge1[1]=3;
			edge1[2]=1;
			edge1[3]=5;
			corner2[0]=6;
			corner2[1]=0;
			corner2[2]=2;
			corner2[3]=8;
			break;
		case 5:
			for(i=0;i<4;i++)
			{
				corner1[i]=8;
				corner2[i]=6;
				edge1[i]=7;
			}
			break;
	}
}
void Moves :: changestate()
{
	int t[5],i,j;
	dblptr x1;
	Adjface *p;
	selectsquares(move_no);
	x1=Face :: getadjface(move_no);
	if(clock==0)
	{
		p=x1.head2;
		for(i=3;i>0;i--)
		{
			if(i==3)
			{
					t[0]=rub->getface(9*move_no+corner[i]).getrow(9*move_no+corner[i]).getsquare(9*move_no+corner[i]).getcolor();
					t[1]=rub->getface(9*move_no+edge[i]).getrow(9*move_no+edge[i]).getsquare(9*move_no+edge[i]).getcolor();
			}
			rub->getface(9*move_no+corner[i]).getrow(9*move_no+corner[i]).getsquare(9*move_no+corner[i]).setcolor(rub->getface(9*move_no+corner[i-1]).getrow(9*move_no+corner[i-1]).getsquare(9*move_no+corner[i-1]).getcolor());
			rub->getface(9*move_no+edge[i]).getrow(9*move_no+edge[i]).getsquare(9*move_no+edge[i]).setcolor(rub->getface(9*move_no+edge[i-1]).getrow(9*move_no+edge[i-1]).getsquare(9*move_no+edge[i-1]).getcolor());
		}
		rub->getface(9*move_no+corner[i]).getrow(9*move_no+corner[i]).getsquare(9*move_no+corner[i]).setcolor(t[0]);
		rub->getface(9*move_no+edge[i]).getrow(9*move_no+edge[i]).getsquare(9*move_no+edge[i]).setcolor(t[1]);
	}
	if(clock==1)
	{
		p=x1.head1;
		for(i=0;i<3;i++)
		{
			if(i==0)
			{
					t[0]=rub->getface(9*move_no+corner[i]).getrow(9*move_no+corner[i]).getsquare(9*move_no+corner[i]).getcolor();
					t[1]=rub->getface(9*move_no+edge[i]).getrow(9*move_no+edge[i]).getsquare(9*move_no+edge[i]).getcolor();
			}
			rub->getface(9*move_no+corner[i]).getrow(9*move_no+corner[i]).getsquare(9*move_no+corner[i]).setcolor(rub->getface(9*move_no+corner[i+1]).getrow(9*move_no+corner[i+1]).getsquare(9*move_no+corner[i+1]).getcolor());
			rub->getface(9*move_no+edge[i]).getrow(9*move_no+edge[i]).getsquare(9*move_no+edge[i]).setcolor(rub->getface(9*move_no+edge[i+1]).getrow(9*move_no+edge[i+1]).getsquare(9*move_no+edge[i+1]).getcolor());
		}
		rub->getface(9*move_no+corner[i]).getrow(9*move_no+corner[i]).getsquare(9*move_no+corner[i]).setcolor(t[0]);
		rub->getface(9*move_no+edge[i]).getrow(9*move_no+edge[i]).getsquare(9*move_no+edge[i]).setcolor(t[1]);
	}
	if((clock==0 && (move_no==0 || move_no==2 || move_no==3)) || (clock==1 && (move_no==1 || move_no==4 || move_no==5)))
	{
		p=x1.head2;
		for(i=3;i>0;i--)
		{
			if(i==3)
			{
					t[2]=rub->getface(9*p->num+corner1[i]).getrow(9*p->num+corner1[i]).getsquare(9*p->num+corner1[i]).getcolor();
					t[3]=rub->getface(9*p->num+edge1[i]).getrow(9*p->num+edge1[i]).getsquare(9*p->num+edge1[i]).getcolor();
					t[4]=rub->getface(9*p->num+corner2[i]).getrow(9*p->num+corner2[i]).getsquare(9*p->num+corner2[i]).getcolor();
			}
			rub->getface(9*p->num+corner1[i]).getrow(9*p->num+corner1[i]).getsquare(9*p->num+corner1[i]).setcolor(rub->getface(9*p->prev->num+corner1[i-1]).getrow(9*p->prev->num+corner1[i-1]).getsquare(9*p->prev->num+corner1[i-1]).getcolor());
			rub->getface(9*p->num+edge1[i]).getrow(9*p->num+edge1[i]).getsquare(9*p->num+edge1[i]).setcolor(rub->getface(9*p->prev->num+edge1[i-1]).getrow(9*p->prev->num+edge1[i-1]).getsquare(9*p->prev->num+edge1[i-1]).getcolor());
			rub->getface(9*p->num+corner2[i]).getrow(9*p->num+corner2[i]).getsquare(9*p->num+corner2[i]).setcolor(rub->getface(9*p->prev->num+corner2[i-1]).getrow(9*p->prev->num+corner2[i-1]).getsquare(9*p->prev->num+corner2[i-1]).getcolor());
			p=p->prev;
		}
		rub->getface(9*p->num+corner1[i]).getrow(9*p->num+corner1[i]).getsquare(9*p->num+corner1[i]).setcolor(t[2]);
		rub->getface(9*p->num+edge1[i]).getrow(9*p->num+edge1[i]).getsquare(9*p->num+edge1[i]).setcolor(t[3]);
		rub->getface(9*p->num+corner2[i]).getrow(9*p->num+corner2[i]).getsquare(9*p->num+corner2[i]).setcolor(t[4]);
	}
	if((clock==1 && (move_no==0 || move_no==2 || move_no==3)) || (clock==0 && (move_no==1 || move_no==4 || move_no==5)))
	{
		p=x1.head1;
		for(i=0;i<3;i++)
		{
			if(i==0)
			{
					t[2]=rub->getface(9*p->num+corner1[i]).getrow(9*p->num+corner1[i]).getsquare(9*p->num+corner1[i]).getcolor();
					t[3]=rub->getface(9*p->num+edge1[i]).getrow(9*p->num+edge1[i]).getsquare(9*p->num+edge1[i]).getcolor();
					t[4]=rub->getface(9*p->num+corner2[i]).getrow(9*p->num+corner2[i]).getsquare(9*p->num+corner2[i]).getcolor();
			}
			rub->getface(9*p->num+corner1[i]).getrow(9*p->num+corner1[i]).getsquare(9*p->num+corner1[i]).setcolor(rub->getface(9*p->next->num+corner1[i+1]).getrow(9*p->next->num+corner1[i+1]).getsquare(9*p->next->num+corner1[i+1]).getcolor());
			rub->getface(9*p->num+edge1[i]).getrow(9*p->num+edge1[i]).getsquare(9*p->num+edge1[i]).setcolor(rub->getface(9*p->next->num+edge1[i+1]).getrow(9*p->next->num+edge1[i+1]).getsquare(9*p->next->num+edge1[i+1]).getcolor());
			rub->getface(9*p->num+corner2[i]).getrow(9*p->num+corner2[i]).getsquare(9*p->num+corner2[i]).setcolor(rub->getface(9*p->next->num+corner2[i+1]).getrow(9*p->next->num+corner2[i+1]).getsquare(9*p->next->num+corner2[i+1]).getcolor());
			p=p->next;
		}
		rub->getface(9*p->num+corner1[i]).getrow(9*p->num+corner1[i]).getsquare(9*p->num+corner1[i]).setcolor(t[2]);
		rub->getface(9*p->num+edge1[i]).getrow(9*p->num+edge1[i]).getsquare(9*p->num+edge1[i]).setcolor(t[3]);
		rub->getface(9*p->num+corner2[i]).getrow(9*p->num+corner2[i]).getsquare(9*p->num+corner2[i]).setcolor(t[4]);
	}
}
Cube* Moves :: getcube()
{
	return rub;
}
int* Moves :: getedge1()
{
	return edge1;
}
int* Moves :: getcorner1()
{
	return corner1;
}
int* Moves :: getcorner2()
{
	return corner2;
}
int Moves :: getmove()
{
	return move_no;
}
int Moves :: getclock()
{
	return clock;
}
class Shape
{
	public:
		virtual void draw(int)=0;
};
class Quad : public Shape
{
	private:
		int x1,y1,x2,y2,x3,y3,x4,y4;
	public:
		void setvalues(int,int,int,int);
		void setvalues(int*);
		void draw(int);
};
void Quad :: setvalues(int a,int b,int c,int d)
{
	x1=x4=a;
	x2=x3=c;
	y1=y2=b;
	y3=y4=d;
}
void Quad :: setvalues(int *a)
{
	x1=a[0];
	x2=a[2];
	x3=a[4];
	x4=a[6];
	y1=a[1];
	y2=a[3];
	y3=a[5];
	y4=a[7];
}
void Quad :: draw(int x)
{
	int poly[10];
	poly[0]=x1;
	poly[1]=y1;
	poly[2]=x2;
	poly[3]=y2;
	poly[4]=x3;
	poly[5]=y3;
	poly[6]=x4;
	poly[7]=y4;
	poly[8]=x1;
	poly[9]=y1;
	if(x==0)
		drawpoly(5,poly);
	else
		fillpoly(5,poly);
}
class Arrow : public Shape
{
	private:
		int radius,angle,end,x,y;
	public:
		void setvalues(int,int,int,int,int);
		void draw(int);
};
void Arrow :: setvalues(int x,int y,int angle,int end,int radius)
{
	this->x=x;
	this->y=y;
	this->radius=radius;
	this->angle=angle;
	this->end=end;
}
void Arrow :: draw(int c)
{
	int i;
	double a,b;
	setlinestyle(0,0,3);
	arc(x,y,angle+5,end-5,radius);
	a=(double)x-(double)radius*cos(90);
	if(c==0)
	{
		b=(double)y-(double)radius*sin(90);
		line(a,b,a-10,b-20);
		line(a,b,a-20,b+10);
	}
	else if(c==1)
	{
		b=(double)y+(double)radius*sin(90);
		line(a,b,a-10,b+20);
		line(a,b,a-20,b-10);
	}
}
class Draw
{
	private:
		Shape *s;
	public:
		Draw();
		void setshape(Shape*);
		void drawshape(int);
};
Draw :: Draw()
{
	s=NULL;
}
void Draw :: setshape(Shape *x)
{
	s=x;
}
void Draw :: drawshape(int x)
{
	s->draw(x);
}
struct Linkmoves
{
	Moves a;
	Linkmoves *next;
};
template <class T>
	class Stack
	{
		private:
			T *top;
		public:
			Stack();
			int empty();
			void push(T*);
			T* pop();
	};
	template <class T> Stack<T> :: Stack()
	{
		top=NULL;
	}
	template <class T> int Stack<T> :: empty()
	{
		if(top==NULL)
			return 1;
		else
			return 0;
	}
	template <class T> void Stack<T> :: push(T* temp)
	{
		temp->next=top;
		top=temp;
	}
	template <class T> T* Stack<T> :: pop()
	{
			T* temp=top;
			top=top->next;
			return temp;
	}
template <class T>
	T* allotspace(T *x,int size)
	{
		x=new T[size];
		if(x==NULL)
			/*throw*/ cout<<"The required memory wasnt alloted";
		return x;
	}
template <class T>
	T* allotspace(T *x)
	{
		x=new T;
		if(x==NULL)
			/*throw*/ cout<<"The memory wasnt alloted to the desired object\n";
		return x;
	}
/*template<>
	Moves* allotspace<Moves>(Moves *x)
	{
		x=new Moves;
		if(x==NULL)
			throw "The Move object wasn't alloted the memory";
		return x;
	}*/
int curclr,validate,backspace=0;
Stack<Linkmoves> stck;
void mouseinitialize();
void hidepointer();
void getmouseposition();
void cube2d(Cube*);
void buttons1(Cube&);
int getposition(int,int);
void click1(Cube&);
void allphase(Cube&,Cube&,Cube&,Cube&,Cube&,Cube&,Cube&,Cube&);
void sp1p1(int,int,int);
void sp1p2(int);
void sp1p3(int);
void sp1p4(int);
void solphase1(Cube&);
int edgepiecebase(int);
int sp2p1(int);
void sp2p2(int,int,int);
void sp2p3(int,int,int);
void solphase2(Cube&);
void sp3p1(int,int);
void solphase3(Cube&);
int* cornerpiecebase(int);
void sp4p1(Adjface*);
void sp4p2(Adjface*);
void solphase4(Cube&);
void sp5p1(Adjface*);
void solphase5(Cube&);
void sp6p1(Adjface*);
void solphase6(Cube&);
void sp7p1(Adjface*,int);
void solphase7(Cube&);
void movingtemplate(int,int);
int findface(int);
void drawarrow(int);
void addprevstate(int,int);
int gotoprevstate();
int main()
{
	int mode,driver=DETECT,i,j,*x;
	x=allotspace(x,54);
	for(i=0;i<54;i++)
		x[i]=i;
	Cube rubik(x),phase1,phase2,phase3,phase4,phase5,phase6,phase7;
	Face :: createadjface();
	curclr=validate=0;
	initgraph(&driver,&mode,"c:\\turboc3\\bgi");
	settextstyle(2,0,5);
	settextjustify(1,1);
	outtextxy(getmaxx()/2,100,"Enter the current state of the rubiks cube");
	buttons1(rubik);
	do
	{
		cube2d(&rubik);
		mouseinitialize();
		click1(rubik);
		hidepointer();
	}while(/*rubik.getcount()!=54 || */validate==0);
	Moves :: addcube(&rubik);
	for(i=0;i<6;i++)
	{
		j=i*9+4;
		Moves::getcube()->getface(j).setface_color(rubik.getface(j).getrow(j).getsquare(j).getcolor());
	}
	allphase(rubik,phase1,phase2,phase3,phase4,phase5,phase6,phase7);
	solphase1(phase1);
	solphase2(phase2);
	solphase3(phase3);
	solphase4(phase4);
	solphase5(phase5);
	solphase6(phase6);
	solphase7(phase7);
	closegraph();
	getch();
	return 0;
}
void mouseinitialize()
{
	i.x.ax=0;
	int86(0X33,&i,&o);
	if(o.x.ax!=0)
	{
		i.x.ax=1;
		int86(0X33,&i,&o);
	}
}
void hidepointer()
{
	i.x.ax=2;
	int86(0X33,&i,&o);
}
void getmouseposition()
{
	do
	{
		i.x.ax=3;
		int86(0X33,&i,&o);
		m.button=o.x.bx;
		m.x=o.x.cx;
		m.y=o.x.dx;
	}while(!kbhit() && m.button==0);
}
void cube2d(Cube *rubik)
{
	int x;
	Draw *a;
	/*try
	{
		a=allotspace(a);
	}
	catch(const char* msg)
	{
		cerr<<msg<<endl;
	}*/
	a=allotspace(a);
	Quad s;
	a->setshape(&s);
	setlinestyle(0,0,1);
	for(int i=-9;i<3;i++)
		for(int j=0;j<3;j++)
		{
			x=getposition(getmaxx()/2+i*30+15,getmaxy()/2+j*30+15);
			setfillstyle(1,rubik->getface(x).getrow(x).getsquare(x).getcolor());
			s.setvalues(getmaxx()/2+i*30,getmaxy()/2+j*30,getmaxx()/2+(i+1)*30,getmaxy()/2+(j+1)*30);
			a->drawshape(1);
			if(i>=-6 && i<-3)
			{
				x=getposition(getmaxx()/2+i*30+15,getmaxy()/2+j*30-75);
				setfillstyle(1,rubik->getface(x).getrow(x).getsquare(x).getcolor());
				s.setvalues(getmaxx()/2+i*30,getmaxy()/2-90+j*30,getmaxx()/2+(i+1)*30,getmaxy()/2-90+(j+1)*30);
				a->drawshape(1);
				x=getposition(getmaxx()/2+i*30+15,getmaxy()/2+j*30+105);
				setfillstyle(1,rubik->getface(x).getrow(x).getsquare(x).getcolor());
				s.setvalues(getmaxx()/2+i*30,getmaxy()/2+90+j*30,getmaxx()/2+(i+1)*30,getmaxy()/2+90+(j+1)*30);
				a->drawshape(1);
			}
		}
	setlinestyle(0,0,3);
	s.setvalues(getmaxx()/2-270,getmaxy()/2,getmaxx()/2+90,getmaxy()/2+90);
	a->drawshape(0);
	s.setvalues(getmaxx()/2-90,getmaxy()/2-90,getmaxx()/2-180,getmaxy()/2+180);
	a->drawshape(0);
	line(getmaxx()/2,getmaxy()/2,getmaxx()/2,getmaxy()/2+90);
}
void buttons1(Cube &rubik)
{
	int col;
	char colname[7];
	Draw *a;
	/*try
	{
		a=allotspace(a);
	}
	catch(const char* msg)
	{
		cerr<<msg<<endl;
	}*/
	a=allotspace(a);
	Quad s;
	a->setshape(&s);
	setlinestyle(0,0,1);
	for(int i=0;i<6;i++)
	{
		s.setvalues(getmaxx()/2+180,getmaxy()/2-90+i*45,getmaxx()/2+250,getmaxy()/2-60+i*45);
		a->drawshape(0);
		switch(i)
		{
			case 0:
				col=1;
				strcpy(colname,"Blue");
				break;
			case 1:
				col=2;
				strcpy(colname,"Green");
				break;
			case 2:
				col=4;
				strcpy(colname,"Red");
				break;
			case 3:
				col=13;
				strcpy(colname,"Pink");
				break;
			case 4:
				col=14;
				strcpy(colname,"Yellow");
				break;
			case 5:
				col=15;
				strcpy(colname,"White");
				break;
		}
		setfillstyle(1,col);
		s.setvalues(getmaxx()/2+230,getmaxy()/2-80+i*45,getmaxx()/2+240,getmaxy()/2-70+i*45);
		a->drawshape(1);
		setcolor(col);
		outtextxy(getmaxx()/2+205,getmaxy()/2-75+i*45,colname);
		setcolor(WHITE);
	}
	if(rubik.getcount()!=54)
		setcolor(DARKGRAY);
	else
		setcolor(WHITE);
	s.setvalues(getmaxx()/2-35,430,getmaxx()/2+35,460);
	a->drawshape(0);
	outtextxy(getmaxx()/2,445,"Validate");
	setcolor(WHITE);
}
int getposition(int a,int b)
{
	if(a>getmaxx()/2-270 && a<getmaxx()/2-240)
	{
		if(b>getmaxy()/2 && b<getmaxy()/2+30)
			return 9;
		if(b>getmaxy()/2+30 && b<getmaxy()/2+60)
			return 12;
		if(b>getmaxy()/2+60 && b<getmaxy()/2+90)
			return 15;
	}
	if(a>getmaxx()/2-240 && a<getmaxx()/2-210)
	{
		if(b>getmaxy()/2 && b<getmaxy()/2+30)
			return 10;
		if(b>getmaxy()/2+30 && b<getmaxy()/2+60)
			return 13;
		if(b>getmaxy()/2+60 && b<getmaxy()/2+90)
			return 16;
	}
	if(a>getmaxx()/2-210 && a<getmaxx()/2-180)
	{
		if(b>getmaxy()/2 && b<getmaxy()/2+30)
			return 11;
		if(b>getmaxy()/2+30 && b<getmaxy()/2+60)
			return 14;
		if(b>getmaxy()/2+60 && b<getmaxy()/2+90)
			return 17;
	}
	if(a>getmaxx()/2-180 && a<getmaxx()/2-150)
	{
		if(b>getmaxy()/2 && b<getmaxy()/2+30)
			return 18;
		if(b>getmaxy()/2+30 && b<getmaxy()/2+60)
			return 21;
		if(b>getmaxy()/2+60 && b<getmaxy()/2+90)
			return 24;
		if(b>getmaxy()/2-90 && b<getmaxy()/2-60)
			return 0;
		if(b>getmaxy()/2-60 && b<getmaxy()/2-30)
			return 3;
		if(b>getmaxy()/2-30 && b<getmaxy()/2)
			return 6;
		if(b>getmaxy()/2+90 && b<getmaxy()/2+120)
			return 45;
		if(b>getmaxy()/2+120 && b<getmaxy()/2+150)
			return 48;
		if(b>getmaxy()/2+150 && b<getmaxy()/2+180)
			return 51;

	}
	if(a>getmaxx()/2-150 && a<getmaxx()/2-120)
	{
		if(b>getmaxy()/2 && b<getmaxy()/2+30)
			return 19;
		if(b>getmaxy()/2+30 && b<getmaxy()/2+60)
			return 22;
		if(b>getmaxy()/2+60 && b<getmaxy()/2+90)
			return 25;
		if(b>getmaxy()/2-90 && b<getmaxy()/2-60)
			return 1;
		if(b>getmaxy()/2-60 && b<getmaxy()/2-30)
			return 4;
		if(b>getmaxy()/2-30 && b<getmaxy()/2)
			return 7;
		if(b>getmaxy()/2+90 && b<getmaxy()/2+120)
			return 46;
		if(b>getmaxy()/2+120 && b<getmaxy()/2+150)
			return 49;
		if(b>getmaxy()/2+150 && b<getmaxy()/2+180)
			return 52;
	}
	if(a>getmaxx()/2-120 && a<getmaxx()/2-90)
	{
		if(b>getmaxy()/2 && b<getmaxy()/2+30)
			return 20;
		if(b>getmaxy()/2+30 && b<getmaxy()/2+60)
			return 23;
		if(b>getmaxy()/2+60 && b<getmaxy()/2+90)
			return 26;
		if(b>getmaxy()/2-90 && b<getmaxy()/2-60)
			return 2;
		if(b>getmaxy()/2-60 && b<getmaxy()/2-30)
			return 5;
		if(b>getmaxy()/2-30 && b<getmaxy()/2)
			return 8;
		if(b>getmaxy()/2+90 && b<getmaxy()/2+120)
			return 47;
		if(b>getmaxy()/2+120 && b<getmaxy()/2+150)
			return 50;
		if(b>getmaxy()/2+150 && b<getmaxy()/2+180)
			return 53;
	}
	if(a>getmaxx()/2-90 && a<getmaxx()/2-60)
	{
		if(b>getmaxy()/2 && b<getmaxy()/2+30)
			return 27;
		if(b>getmaxy()/2+30 && b<getmaxy()/2+60)
			return 30;
		if(b>getmaxy()/2+60 && b<getmaxy()/2+90)
			return 33;
	}
	if(a>getmaxx()/2-60 && a<getmaxx()/2-30)
	{
		if(b>getmaxy()/2 && b<getmaxy()/2+30)
			return 28;
		if(b>getmaxy()/2+30 && b<getmaxy()/2+60)
			return 31;
		if(b>getmaxy()/2+60 && b<getmaxy()/2+90)
			return 34;
	}
	if(a>getmaxx()/2-30 && a<getmaxx()/2)
	{
		if(b>getmaxy()/2 && b<getmaxy()/2+30)
			return 29;
		if(b>getmaxy()/2+30 && b<getmaxy()/2+60)
			return 32;
		if(b>getmaxy()/2+60 && b<getmaxy()/2+90)
			return 35;
	}
	if(a>getmaxx()/2 && a<getmaxx()/2+30)
	{
		if(b>getmaxy()/2 && b<getmaxy()/2+30)
			return 36;
		if(b>getmaxy()/2+30 && b<getmaxy()/2+60)
			return 39;
		if(b>getmaxy()/2+60 && b<getmaxy()/2+90)
			return 42;
	}
	if(a>getmaxx()/2+30 && a<getmaxx()/2+60)
	{
		if(b>getmaxy()/2 && b<getmaxy()/2+30)
			return 37;
		if(b>getmaxy()/2+30 && b<getmaxy()/2+60)
			return 40;
		if(b>getmaxy()/2+60 && b<getmaxy()/2+90)
			return 43;
	}
	if(a>getmaxx()/2+60 && a<getmaxx()/2+90)
	{
		if(b>getmaxy()/2 && b<getmaxy()/2+30)
			return 38;
		if(b>getmaxy()/2+30 && b<getmaxy()/2+60)
			return 41;
		if(b>getmaxy()/2+60 && b<getmaxy()/2+90)
			return 44;
	}
}
void click1(Cube &rubik)
{
	int k;
	cmp:
	getmouseposition();
	if(m.x>getmaxx()/2+180 && m.x<getmaxx()/2+250)
	{
		if(m.y>getmaxy()/2-90 && m.y<getmaxy()/2-60)
			curclr=1;
		else if(m.y>getmaxy()/2-45 && m.y<getmaxy()/2-15)
			curclr=2;
		else if(m.y>getmaxy()/2 && m.y<getmaxy()/2+30)
			curclr=4;
		else if(m.y>getmaxy()/2+45 && m.y<getmaxy()/2+75)
			curclr=13;
		else if(m.y>getmaxy()/2+90 && m.y<getmaxy()/2+120)
			curclr=14;
		else if(m.y>getmaxy()/2+135 && m.y<getmaxy()/2+165)
			curclr=15;
		goto cmp;
	}
	else if(((m.x>getmaxx()/2-270 && m.x<getmaxx()/2+90 && m.y>getmaxy()/2 && m.y<getmaxy()/2+90) || (m.x>getmaxx()/2-180 && m.x<getmaxx()/2-90 && m.y>getmaxy()/2-90 && m.y<getmaxy()/2+180)) && curclr!=0)
	{
		k=getposition(m.x,m.y);
		if(rubik.getface(k).getrow(k).getsquare(k).getcolor()==0)
			rubik++;
		rubik.getface(k).getrow(k).getsquare(k).setcolor(curclr);
	}
	else if(m.x>getmaxx()/2-35 && m.x<getmaxx()/2+35 && m.y>430 && m.y<460)// && rubik.getcount()==54)
		validate=1;
	else
		goto cmp;
}
void allphase(Cube &inp,Cube &p1,Cube &p2,Cube &p3,Cube &p4,Cube &p5,Cube &p6,Cube &p7)
{
	int i;
	for(i=4;i<54;i+=9)
		p1.getface(i).getrow(i).getsquare(i).setcolor(inp.getface(i).getrow(i).getsquare(i).getcolor());
	for(i=0;i<54;i++)
	{
		if((i%9==3 || i%9==5 || i%9==7) && i/9==0)
			p1.getface(i).getrow(i).getsquare(i).setcolor(p1.getface(4).getrow(4).getsquare(4).getcolor());
		if(i%9==1 && i/9!=5)
			p1.getface(i).getrow(i).getsquare(i).setcolor(p1.getface((i/9)*9+4).getrow((i/9)*9+4).getsquare((i/9)*9+4).getcolor());
		p2.getface(i).getrow(i).getsquare(i).setcolor(p1.getface(i).getrow(i).getsquare(i).getcolor());
	}
	for(i=0;i<54;i++)
	{
		if((i%9==6 || i%9==8) && i/9==0)
			p2.getface(i).getrow(i).getsquare(i).setcolor(p2.getface(4).getrow(4).getsquare(4).getcolor());
		if((i%9==0 || i%9==2) && i/9!=5)
			p2.getface(i).getrow(i).getsquare(i).setcolor(p2.getface((i/9)*9+4).getrow((i/9)*9+4).getsquare((i/9)*9+4).getcolor());
		p3.getface(i).getrow(i).getsquare(i).setcolor(p2.getface(i).getrow(i).getsquare(i).getcolor());
	}
	for(i=0;i<54;i++)
	{
		if((i%9==3 || i%9==5) && (i/9!=5 && i/9!=0))
			p3.getface(i).getrow(i).getsquare(i).setcolor(p3.getface((i/9)*9+4).getrow((i/9)*9+4).getsquare((i/9)*9+4).getcolor());
		p4.getface(i).getrow(i).getsquare(i).setcolor(p3.getface(i).getrow(i).getsquare(i).getcolor());
		p5.getface(i).getrow(i).getsquare(i).setcolor(p3.getface(i).getrow(i).getsquare(i).getcolor());
		p6.getface(i).getrow(i).getsquare(i).setcolor(p3.getface(i).getrow(i).getsquare(i).getcolor());
	}
	for(i=45;i<54;i++)
	{
		if((i%9)%2!=0)
			p4.getface(i).getrow(i).getsquare(i).setcolor(p4.getface(49).getrow(49).getsquare(49).getcolor());
		p5.getface(i).getrow(i).getsquare(i).setcolor(p4.getface(i).getrow(i).getsquare(i).getcolor());
		if((i%9)%2==0)
			p5.getface(i).getrow(i).getsquare(i).setcolor(p5.getface(49).getrow(49).getsquare(49).getcolor());
		p6.getface(i).getrow(i).getsquare(i).setcolor(p5.getface(i).getrow(i).getsquare(i).getcolor());
	}
	for(i=9;i<45;i++)
	{
		if(i%9==6 || i%9==8)
			p6.getface(i).getrow(i).getsquare(i).setcolor(p6.getface((i/9)*9+4).getrow((i/9)*9+4).getsquare((i/9)*9+4).getcolor());
	}
	for(i=0;i<54;i++)
		p7.getface(i).getrow(i).getsquare(i).setcolor(inp.getface((i/9)*9+4).getrow((i/9)*9+4).getsquare((i/9)*9+4).getcolor());
}
void drawarrow(int c)
{
	Draw b;
	Arrow x;
	x.setvalues(getmaxx()/2+220,getmaxy()/2,60,300,50);
	b.setshape(&x);
	b.drawshape(c);
}
void addprevstate(int m,int c)
{
	Linkmoves *temp;
	temp=allotspace(temp);
	temp->a.setmove(m,c);
	stck.push(temp);
}
int gotoprevstate()
{
	Linkmoves *temp;
	if(!stck.empty())
	{
		temp=stck.pop();
		movingtemplate(temp->a.getmove(),temp->a.getclock());
		return 1;
	}
	else
		return 0;
}
void movingtemplate(int m,int c)
{
	Moves *a;
	/*try
	{
		a=allotspace(a);
	}
	catch(const char* msg)
	{
		cerr<<msg<<endl;
	}*/
	a=allotspace(a);
	char s[2],d;
	int x;
	a->setmove(m,c);
	a->changestate();
	if(backspace==0)
		addprevstate(m,!c);
	cleardevice();
	cube2d(Moves::getcube());
	settextstyle(2,0,5);
	outtextxy(getmaxx()/2,getmaxy()/2+200,"Press Any Key For Next Step");
	outtextxy(getmaxx()/2,getmaxy()/2-200,"Press Backspace For Previous Step");
	settextstyle(3,0,4);
	s[0]=a->getinitials();
	s[1]='\0';
	outtextxy(getmaxx()/2+220,getmaxy()/2,s);
	drawarrow(c);
	q:
	d=getch();
	if(d=='\b')
	{
		backspace=1;
		x=gotoprevstate();
		backspace=0;
		if(x==0)
			goto q;
	}
}
void sp1p1(int i,int x,int z)
{
	dblptr l;
	Adjface *p;
	l=Face::getadjface(0);
	p=l.head1;
	while(p->num!=x)
		p=p->next;
	if(p->prev->num==z)
	{
		switch(i)
		{
			case 0:
				movingtemplate(0,1);
				break;
			case 5:
				movingtemplate(5,0);
				break;
		}
	}
	else
	{
		do
		{
			switch(i)
			{
				case 0:
					movingtemplate(0,0);
					break;
				case 5:
					movingtemplate(5,1);
					break;
			}
			p=p->next;
		}while(p->num!=z);
	}
}
void sp1p2(int z)
{
	for(int w=0;w<2;w++)
		movingtemplate(z,0);
}
void sp1p3(int z)
{
	dblptr l;
	Adjface *p;
	movingtemplate(0,0);
	l=Face::getadjface(0);
	p=l.head1;
	while(p->num!=z)
		p=p->next;
	movingtemplate(p->next->num,0);
	movingtemplate(0,1);
	movingtemplate(z,0);
}
void sp1p4(int z, int k)
{
	movingtemplate(z,k);
	movingtemplate(5,0);
	movingtemplate(z,!k);
}
void solphase1(Cube& endstate)
{
	int tcolor=Moves::getcube()->getface(0).getface_color(),i=0,x,z,j,n=0,previous;
	Moves a;
	while(!(endstate==Moves::getcube()))
	{
		if((i%9)%2==1)
		{
			if(Moves::getcube()->getface(i).getrow(i).getsquare(i).getcolor()==tcolor && i!=previous)
			{
				x=edgepiecebase(i);
				z=findface(x);
				if(x/9!=z || i/9!=0)
				{
					if(i/9==0)
					{
						if(n==0)
						{
							sp1p1(i/9,x/9,z);
							n++;
						}
						else if(n>0)
							sp1p2(x/9);
					}
					else if(i/9==5)
					{
						sp1p1(i/9,x/9,z);
						sp1p2(z);
						n++;
					}
					else
					{
						if(i%9==1)
						{
							if(i/9!=z)
							{
								if(n!=0)
									sp1p2(i/9);
								else
								{
									sp1p1(0,i/9,z);
									n++;
								}
							}
							else
							{
								sp1p3(z);
								n++;
							}
						}
						else if(i%9==7)
						{
							if(i/9!=z)
								sp1p1(5,i/9,z);
							else
								sp1p2(z);
						}
						else if(i%9==3)
						{
							if(x/9==z)
							{
								movingtemplate(z,1);
								n++;
							}
							else
								sp1p4(i/9,1);
						}
						else
						{       if(x/9==z)
							{
								movingtemplate(z,0);
								n++;
							}
							else
								sp1p4(i/9,0);
						}
					}
					previous=i;
					i=0;
				}
			}
		}
		i++;
	}
}
int edgepiecebase(int sqr_no)
{
	int corr_sqr_no,face_num,*y,k;
	dblptr x;
	Adjface *p;
	face_num=sqr_no/9;
	x=Face :: getadjface(face_num);
	Moves :: selectsquares(face_num);
	y=Moves :: getedge1();
	switch(sqr_no%9)
	{
		case 1:
			p=x.head1->next->next;
			k=2;
			break;
		case 3:
			p=x.head1->next;
			k=1;
			break;
		case 5:
			p=x.head1->prev;
			k=3;
			break;
		case 7:
			p=x.head1;
			k=0;
			break;
	}
	return(p->num*9+y[k]);
}
int sp2p1(int i)
{
	int *x,z[2],j,k;
	x=cornerpiecebase(i);
	if(Moves::getcube()->getface(i).getrow(i).getsquare(i).getcolor()==Moves::getcube()->getface(0).getface_color())
	{
		for(k=0;k<2;k++)
			for(j=1;j<5;j++)
				if(Moves::getcube()->getface(x[k]).getrow(x[k]).getsquare(x[k]).getcolor()==Moves::getcube()->getface(j*9).getface_color())
					z[k]=Moves::getcube()->getface(j*9).getface_no();
		if(x[1]/9==z[1] && x[0]/9==z[0])
			return 0;
		else
			return 1;
	}
	return 1;
}
void sp2p2(int m,int c,int d)
{
	movingtemplate(m,c);
	movingtemplate(5,d);
	movingtemplate(m,!c);
}
void sp2p3(int i, int z, int k)
{
	dblptr s;
	Adjface *p;
	s=Face::getadjface(0);
	p=s.head1;
	while(p->num!=i)
		p=p->next;
	if(p->prev->num==z)
	{
		movingtemplate(5,1);
		sp2p2(z,k,!k);
	}
	else if(p->num==z)
	{
		movingtemplate(5,0);
		movingtemplate(5,0);
		sp2p2(z,k,!k);
	}
	else if(p->next->num==z)
	{
		movingtemplate(5,0);
		sp2p2(z,k,!k);
	}
	else if(p->next->next->num==z)
		sp2p2(z,k,!k);
}
void solphase2(Cube &endstate)
{
	int tcolor=Moves::getcube()->getface(0).getface_color(),i=0,*a,x[2],j,z[2],k,r,previous;
	while(!(endstate==Moves::getcube()))
	{
		if((i%9)%2==0 && i%9!=4)
		{
			if(Moves::getcube()->getface(i).getrow(i).getsquare(i).getcolor()==tcolor && i!=previous)
			{
				a=cornerpiecebase(i);
				for(k=0;k<2;k++)
				{
					x[k]=a[k];
					for(j=1;j<5;j++)
						if(Moves::getcube()->getface(a[k]).getrow(a[k]).getsquare(a[k]).getcolor()==Moves::getcube()->getface(j*9).getface_color())
							z[k]=Moves::getcube()->getface(j*9).getface_no();
				}
				if(i/9!=0 || x[0]/9!=z[0] || x[1]/9!=z[1])
				{
					if(i/9==0)
						sp2p2(x[1]/9,0,0);
					else if(i/9==5)
					{
						switch(i)
						{
							case 45:
								r=6;
								break;
							case 47:
								r=8;
								break;
							case 51:
								r=0;
								break;
							case 53:
								r=2;
								break;
						}
						if(sp2p1(r))
							sp2p2(x[1]/9,1,0);
						else
							movingtemplate(5,0);
					}
					else
					{
						if(i%9==0)
							sp2p2(i/9,1,1);
						else if(i%9==2)
							sp2p2(i/9,0,0);
						else if(i%9==6)
							sp2p3(i/9,z[1],0);
						else
							sp2p3(i/9,z[0],1);
					}
					previous=i;
					i=0;
				}
			}
		}
		i++;
	}
}
int* cornerpiecebase(int sqr_no)
{
	int static corr_sqr_no[2];
	int face_num,*y,*z,k;
	dblptr x;
	face_num=sqr_no/9;
	x=Face :: getadjface(face_num);
	Moves :: selectsquares(face_num);
	y=Moves :: getcorner1();
	z=Moves :: getcorner2();
	if(face_num==0 || face_num==2 || face_num==3)
		switch(sqr_no%9)
		{
			case 0:
				corr_sqr_no[0]=x.head1->next->num*9+y[1];
				corr_sqr_no[1]=x.head1->next->next->num*9+z[2];
				break;
			case 2:
				corr_sqr_no[0]=x.head1->next->next->num*9+y[2];
				corr_sqr_no[1]=x.head1->prev->num*9+z[3];
				break;
			case 6:
				corr_sqr_no[0]=x.head1->num*9+y[0];
				corr_sqr_no[1]=x.head1->next->num*9+z[1];
				break;
			case 8:
				corr_sqr_no[0]=x.head1->prev->num*9+y[3];
				corr_sqr_no[1]=x.head1->num*9+z[0];
				break;
		}
	else if(face_num==1 || face_num==4)
		switch(sqr_no%9)
		{
			case 0:
				corr_sqr_no[0]=x.head1->prev->num*9+y[3];
				corr_sqr_no[1]=x.head1->next->next->num*9+z[2];
				break;
			case 2:
				corr_sqr_no[0]=x.head1->next->next->num*9+y[2];
				corr_sqr_no[1]=x.head1->next->num*9+z[1];
				break;
			case 6:
				corr_sqr_no[0]=x.head1->num*9+y[0];
				corr_sqr_no[1]=x.head1->prev->num*9+z[3];
				break;
			case 8:
				corr_sqr_no[0]=x.head1->next->num*9+y[1];
				corr_sqr_no[1]=x.head1->num*9+z[0];
				break;
		}
	else
		switch(sqr_no%9)
		{
			case 0:
				corr_sqr_no[0]=x.head1->next->num*9+y[1];
				corr_sqr_no[1]=x.head1->num*9+z[0];
				break;
			case 2:
				corr_sqr_no[0]=x.head1->num*9+y[0];
				corr_sqr_no[1]=x.head1->prev->num*9+z[3];
				break;
			case 6:
				corr_sqr_no[0]=x.head1->next->next->num*9+y[2];
				corr_sqr_no[1]=x.head1->next->num*9+z[1];
				break;
			case 8:
				corr_sqr_no[0]=x.head1->prev->num*9+y[3];
				corr_sqr_no[1]=x.head1->next->next->num*9+z[2];
				break;
		}
	return corr_sqr_no;
}
void sp3p1(int i,int z)
{
	dblptr s;
	Adjface *p;
	s=Face::getadjface(0);
	p=s.head1;
	while(p->num!=i)
		p=p->next;
	if(p->next->num==z)
	{
		movingtemplate(5,0);
		movingtemplate(z,0);
		movingtemplate(5,1);
		movingtemplate(z,1);
		movingtemplate(5,1);
		movingtemplate(i,1);
		movingtemplate(5,0);
		movingtemplate(i,0);
	}
	else if(p->prev->num==z)
	{
		movingtemplate(5,1);
		movingtemplate(z,1);
		movingtemplate(5,0);
		movingtemplate(z,0);
		movingtemplate(5,0);
		movingtemplate(i,0);
		movingtemplate(5,1);
		movingtemplate(i,1);
	}
}
void solphase3(Cube &endstate)
{
	int bcolor=Moves::getcube()->getface(5*9).getface_color(),i,k,t,x,z;
	while(!(endstate==Moves::getcube()))
	{
		i=1;
		while(Moves::getcube()->getface(i*9+7).getrow(i*9+7).getsquare(i*9+7).getcolor()==bcolor || Moves::getcube()->getface(edgepiecebase(i*9+7)).getrow(edgepiecebase(i*9+7)).getsquare(edgepiecebase(i*9+7)).getcolor()==bcolor)
		{
			if(i!=4)
				i++;
			else
			{
				t=1;
				while(Moves::getcube()->getface(t*9+5).getrow(t*9+5).getsquare(t*9+5).getcolor()==Moves::getcube()->getface(t*9+5).getface_color() && Moves::getcube()->getface(edgepiecebase(t*9+5)).getrow(edgepiecebase(t*9+5)).getsquare(edgepiecebase(t*9+5)).getcolor()==Moves::getcube()->getface(edgepiecebase(t*9+5)).getface_color())
					t++;
				k=edgepiecebase(t*9+5);
				movingtemplate(k/9,1);
				movingtemplate(5,0);
				movingtemplate(k/9,0);
				movingtemplate(5,0);
				movingtemplate(t,0);
				movingtemplate(5,1);
				movingtemplate(t,1);
				i=1;
			}
		}
		while(Moves::getcube()->getface(i*9+7).getrow(i*9+7).getsquare(i*9+7).getcolor()!=Moves::getcube()->getface(i*9).getface_color())
		{
			movingtemplate(5,0);
			if(i==4)
				i=1;
			else
				i++;
		}
		x=edgepiecebase(i*9+7);
		z=findface(x);
		sp3p1(i,z);
	}
}
int findface(int x)
{
	int z;
	for(int j=1;j<5;j++)
		if(Moves::getcube()->getface(x).getrow(x).getsquare(x).getcolor()==Moves::getcube()->getface(j*9).getface_color())
		{
			z=Moves::getcube()->getface(j*9).getface_no();
			break;
		}
	return z;
}
void sp4p1(Adjface* x)
{
	movingtemplate(x->num,0);
	movingtemplate(5,0);
	movingtemplate(x->next->num,0);
	movingtemplate(5,1);
	movingtemplate(x->next->num,1);
	movingtemplate(x->num,1);
}
void sp4p2(Adjface* x)
{
	movingtemplate(x->num,0);
	movingtemplate(x->next->num,0);
	movingtemplate(5,0);
	movingtemplate(x->next->num,1);
	movingtemplate(5,1);
	movingtemplate(x->num,1);
}
void solphase4(Cube& endstate)
{
	dblptr s;
	Adjface *p;
	s=Face::getadjface(0);
	int bcolor=Moves::getcube()->getface(5*9).getface_color(),j,k;
	while(!(endstate==Moves::getcube()))
	{
		k=0;
		p=s.head1;
		do
		{
			if(Moves::getcube()->getface(edgepiecebase(p->num*9+7)).getrow(edgepiecebase(p->num*9+7)).getsquare(edgepiecebase(p->num*9+7)).getcolor()==bcolor && Moves::getcube()->getface(edgepiecebase(p->next->num*9+7)).getrow(edgepiecebase(p->next->num*9+7)).getsquare(edgepiecebase(p->next->num*9+7)).getcolor()==bcolor)
				k=1;
			else
				p=p->next;
		}while(p!=s.head1 && k==0);
		if(k==1)
			sp4p1(p->next->next);
		else
		{
			j=0;
			p=s.head1;
			do
			{
				if(Moves::getcube()->getface(edgepiecebase(p->num*9+7)).getrow(edgepiecebase(p->num*9+7)).getsquare(edgepiecebase(p->num*9+7)).getcolor()==bcolor && Moves::getcube()->getface(edgepiecebase(p->next->next->num*9+7)).getrow(edgepiecebase(p->next->next->num*9+7)).getsquare(edgepiecebase(p->next->next->num*9+7)).getcolor()==bcolor)
					j=1;
				else
					p=p->next;
			}while(p!=s.head1 && j==0);
			if(j==1)
				sp4p2(p->next);
			else
				sp4p1(s.head1);
		}
	}
}
void sp5p1(Adjface* x)
{
	movingtemplate(x->num,0);
	movingtemplate(5,0);
	movingtemplate(x->num,1);
	movingtemplate(5,0);
	movingtemplate(x->num,0);
	movingtemplate(5,0);
	movingtemplate(5,0);
	movingtemplate(x->num,1);
}
void solphase5(Cube &endstate)
{
	dblptr s;
	Adjface *p;
	s=Face::getadjface(0);
	int bcolor=Moves::getcube()->getface(5*9).getface_color(),j,i,t;
	while(!(endstate==Moves::getcube()))
	{
		j=0;
		for(i=45;i<54;i++)
			if((i%9)%2==0 && i%9!=4 && Moves::getcube()->getface(i).getrow(i).getsquare(i).getcolor()==bcolor)
			{
				j++;
				if(j==1)
					t=i;
			}
		if(j==0)
		{
			p=s.head1;
			while(Moves::getcube()->getface(p->num*9+6).getrow(p->num*9+6).getsquare(p->num*9+6).getcolor()!=bcolor)
				p=p->next;
			sp5p1(p->next->next);
		}
		else if(j==1)
			switch(t)
			{
				case 45:
					sp5p1(s.head1->next);
					break;
				case 47:
					sp5p1(s.head1);
					break;
				case 51:
					sp5p1(s.head1->next->next);
					break;
				case 53:
					sp5p1(s.head1->prev);
					break;
			}
		else if(j==2)
		{
			p=s.head1;
			while(Moves::getcube()->getface(p->num*9+8).getrow(p->num*9+8).getsquare(p->num*9+8).getcolor()!=bcolor)
				p=p->next;
			sp5p1(p->next);
		}
	}
}
void sp6p1(Adjface* x)
{
	movingtemplate(x->next->num,1);
	movingtemplate(x->num,0);
	movingtemplate(x->next->num,1);
	movingtemplate(x->next->next->num,0);
	movingtemplate(x->next->next->num,0);
	movingtemplate(x->next->num,0);
	movingtemplate(x->num,1);
	movingtemplate(x->next->num,1);
	movingtemplate(x->next->next->num,0);
	movingtemplate(x->next->next->num,0);
	movingtemplate(x->next->num,0);
	movingtemplate(x->next->num,0);
	movingtemplate(5,1);
}
void solphase6(Cube &endstate)
{
	dblptr s;
	Adjface *p;
	s=Face::getadjface(0);
	int t;
	while(!(endstate==Moves::getcube()))
	{
		t=0;
		p=s.head1;
		do
		{
			if(Moves::getcube()->getface(p->num*9+8).getrow(p->num*9+8).getsquare(p->num*9+8).getcolor()==Moves::getcube()->getface(p->num*9+6).getrow(p->num*9+6).getsquare(p->num*9+6).getcolor())
				t=1;
			else
				p=p->next;
		}while(p!=s.head1 && t==0);
		if(t==0)
		{
			p=s.head1;
			while(Moves::getcube()->getface(p->num*9+8).getrow(p->num*9+8).getsquare(p->num*9+8).getcolor()!=Moves::getcube()->getface(p->num*9+8).getface_color() && Moves::getcube()->getface(p->prev->num*9+8).getrow(p->prev->num*9+8).getsquare(p->prev->num*9+8).getcolor()!=Moves::getcube()->getface(p->prev->num*9+8).getface_color())
				p=p->next;
			sp6p1(p->next->next);
		}
		if(t==1)
		{
			if(Moves::getcube()->getface(p->num*9+8).getrow(p->num*9+8).getsquare(p->num*9+8).getcolor()==Moves::getcube()->getface(p->prev->num*9).getface_color())
			{
				movingtemplate(5,0);
				p=p->prev;
			}
			else
				while(Moves::getcube()->getface(p->num*9+8).getrow(p->num*9+8).getsquare(p->num*9+8).getcolor()!=Moves::getcube()->getface(p->num*9).getface_color())
				{
					movingtemplate(5,1);
					p=p->next;
				}
			sp6p1(p->next->next);
		}
	}
}
void sp7p1(Adjface *x,int k)
{
	movingtemplate(x->num,0);
	movingtemplate(x->num,0);
	movingtemplate(5,k);
	movingtemplate(x->prev->num,0);
	movingtemplate(x->next->num,1);
	movingtemplate(x->num,0);
	movingtemplate(x->num,0);
	movingtemplate(x->prev->num,1);
	movingtemplate(x->next->num,0);
	movingtemplate(5,k);
	movingtemplate(x->num,0);
	movingtemplate(x->num,0);
}
void solphase7(Cube &endstate)
{
	dblptr s;
	Adjface *p;
	s=Face::getadjface(0);
	int t;
	while(!(endstate==Moves::getcube()))
	{
		t=0;
		p=s.head1;
		do
		{
			if(Moves::getcube()->getface(p->num*9+7).getrow(p->num*9+7).getsquare(p->num*9+7).getcolor()==Moves::getcube()->getface(p->num*9+7).getface_color())
				t=1;
			else
				p=p->next;
		}while(p!=s.head1 && t==0);
		if(t==0)
			sp7p1(s.head1,0);
		else if(t==1)
		{
			if(Moves::getcube()->getface(p->next->num*9+7).getrow(p->next->num*9+7).getsquare(p->next->num*9+7).getcolor()==Moves::getcube()->getface(p->next->next->num*9+7).getface_color())
				sp7p1(p->next->next,1);
			else
				sp7p1(p->next->next,0);
		}
	}
}
