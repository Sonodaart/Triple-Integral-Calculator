


# Triple Integral calculator

##  Table of Contents

- [File Management](#file-management)
- [Usage](#usage)
	- [Compilation](#compilation)
	- [Execution](#execution)
	- [Advanced usages](#advanced-usages)
- [Underlying theory](#underlying-theory)
	- [Mathematical Formulation of the Problem](#mathematical-formulation-of-the-problem)
	- [Numerical Solution](#numerical-solution)
# File Management
Triple Integral calculator
```
│
├── bin // executable 
│ └── integral3D
├── include // headers (.h)
│ ├── error.h
│ ├── linker.h
│ ├── main.h
│ └── math3D.h 
├── lib // library build directory (.o)
├── src // general sources (.cpp)
│ ├── linker.cpp
│ ├── main.cpp
│ └── math3D.cpp
├── test
│ └── function.cpp 
├── Makefile
└── README.md
```
# Usage
### Compilation
To compile the program go in the root of the folder and execute:
```
make
```
In addition it's require to have a shared library to be dynamically linked. To create it write C or C++ file, structured in the following way:
```c++
#ifdef __cplusplus
#define EXPORT_SYMBOL extern "C" __attribute__((visibility("default")))
#else
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif
#include <map>
#include <string>

EXPORT_SYMBOL double f(double x, double y, double z) {
	return 5*x+y;
}

EXPORT_SYMBOL std::map<std::string,double> first = {
	{"x^2",1},
	{"y^2",2},
	{"z^2",1},
	{"r",-10},
	{"<",1}
};

EXPORT_SYMBOL std::map<std::string,double> second = {
	{"y",1},
	{">",1}
};
```
The above code is just an example, more complex code can be written.
To work it's mandatory that the file possesses a function named "f", with same prototype as the one presented in the example, and two maps, one called first and one called second.
The maps are supposed to contain the keys: $x^2, x, y^2, y, z^2, z, r$, and for the inequality $>,<,>=,<=$. The former are the coefficients of the respective terms, while the latter are the inequality, and for those the value assigned is not important. The inequality should correspond to the form $Ax^2+ax+By^2+by+Cz^2+cz+r \gtreqless 0$.
To compile the shared library execute the command:
```bash
g++ -shared -fPIC file_name.cpp -o file_name.so
```
### Execution
The program requires 1 mandatory input, and 3 optional parameters.
The first parameter is the location of the shared library(.so) to be dynamically linked. The second parameter is the tolerance error desired. The third and fourth parameters are the maximum n for the Romberg's algorithm, and the maximum recursion depth for the adaptive integration. A call will look like this:
```bash
PATH_TO_EXECUTABLE/integral3D PATH_TO_SO/function.so 0.1 5 3
```
For more extensive use it's advised to add it to the PATH with:
```bash
export PATH="PATH_TO_PROJECT/bin/:$PATH"
```
If done in the .bashrc it will be available in new instances of the shell.
### Advanced usages
Having the possibility to write actual C or C++ code for the function, extensive use of the ```<cmath>``` library and others can be done.
In addition, it's possible to personalize the domain of integration even further via the use of conditionals.
For instance if it's desired to integrate over a cube of side 1, it's possible to hard code it into the function. For the domain it's used a sensible approximation of the domain(one that contains the wanted domain, but isn't excessively large, since it affects both performances as precision):
```c++
EXPORT_SYMBOL double f(double x, double y, double z){
	if(x==0 and y==0 and z==0){
		return 0;
	}
	if(-0.5<x<0.5 and -0.5<y<0.5 and -0.5<z<0.5){
		return 1/( pow(x,2)+pow(y,2)+pow(z,2) );
	}
	return 0;
}

EXPORT_SYMBOL std::map<std::string,double> first = {
	{"x^2",1},
	{"y^2",1},
	{"z^2",1},
	{"r",-1},
	{"<",1}
};

EXPORT_SYMBOL std::map<std::string,double> second = first;
```
To be noted how it's important to remove eventual points that doesn't belong to the actual domain such as $(0,0,0)$ for this function.
It must be mentioned that even thought this approach add more flexibility, the downside is that the algorithm has to do more work to properly approximate the integral(and also the error may be an underestimate of the actual one). This is because we introduce a discontinuity all around the custom domain, and the algorithm assume the function is continuous.
Additionally it must also be mentioned that domains with sides bigger than MAX_BOUNDED_SIZE (in math3D.cpp) will be cut to have that maximum side length.
# Underlying theory
## Mathematical Formulation of the Problem
The objective of the program is to numerically approximate(with a certain error) the value that would assume the corresponding Lesbegue integration.
We have that the value of the integral is equal to the measure of the volume of the surface. In other words, given a function $f(x,y,z):D\subseteq \mathbb{R}^3 \rightarrow \mathbb{R}^3$, being $D$ a normal regular bounded domain, we have:
$$\iiint_D f(x,y,z)dD = m_{4}(S_f)$$
The function $f$ is provided from the user, and it's supposed that the function has an integral(this means it can also be $\pm\infty$).
The domain $D$ is also given as input, in the form of the intersection of two inequalities of the form $Ax^2+ax+By^2+by+Cz^2+cz+r\gtreqless0$. From this we can see that the border is given by the intersection of two $C^{\infty}(\mathbb{R}^3)$ functions, and thus is also normal and regular.

At this point the aim of the program is to just approximate the value of $m_4(S_f)$(where by $m_4$ it's to be intended the 4D measure function).

## Numerical Solution
The method used to solve the problem relies on Romberg's integration method. 
### Domain
But let's begin by looking at the domain of integration.
Given that the domain is the intersection of two inequalities of the form $Ax^2+ax+By^2+by+Cz^2+cz+r\gtreqless0$. What we do is find the maximum and minimum value assumed by $x,y$ and $z$. After finding each max and min value we look for common intervals. This way we get a rectangular set that contains the domain $D$.
The only thing to do is calculate the values assumed by each coordinate. Let's do the calculations for $x$, and for the others follows with the same logic.
I'm gonna assume that if the inequality is either $<$ or $\leq$, the signs of the coefficients are changed($A\equiv-A,...$), and also the inequality into either $>$ or $\geq$. With this notation it's possible to tackle both $\gtreqless$ at the same time, having to deal only with the case $>$(or $\geq$. Since they're the same let's assume we have $>$.

- Case $A>0$:

	This case is trivial. Given big enough $x$ the inequality holds. Of course there may be intervals in between for which it doesn't hold, but since we want the max and min we immediately know them to be min $=-\infty$ and max $=+\infty$.

From now on it's gonna be assumed that $A<0$.
- Case $B>0 \lor C>0$:

	If any of the 2 are $>0$, we have a situation similar to before, where for every $x$, there exist a $\tilde{y},\tilde{z}$ such that being big enough brings the left side above $0$.
- Case $B<0 \land C<0$:

	Rearranging the terms and practicing a movement of the origin we see that this is the equation of a generic ellipsoid $\frac{x^2}{a^2}+\frac{y^2}{b^2}+\frac{z}{c'^2} < 1$(with here some generic $a,b,c \in \mathbb{R}$).
	We proceed to write $r>-Ax^2-ax-By^2-by-Cz^2-cz$. With this, since we know that $A,B,C<0$ we can change notation once a gain absorbing the signs into the coefficients getting $A \equiv -A, a \equiv -a, ...$ .
	We end up with $Ax^2+ax+By^2+by+Cz^2+cz < r$. We now complete the squares getting $Ax^2+ax+(\sqrt{B}y+\frac{b}{2\sqrt{B}})^2-\frac{b^2}{4B}+(\sqrt{C}y+\frac{c}{2\sqrt{C}})^2-\frac{c^2}{4C} < r$. Since $B,C>0$ the squares of $y$ and $z$ gives values from $0$ to $+\infty$. Since we want the max range of values for $x$ we minor those terms with $0$, getting $Ax^2+ax-\frac{b^2}{4B}-\frac{c^2}{4C} < r$.
	By calling $k\equiv -r-\frac{b^2}{4B}-\frac{c^2}{4C}$, we can rewrite it as $Ax^2+ax+k < 0$. This is a simple second order equation with solutions(remembering that $A>0$):
	- if discriminant $=a^2-4Ak\geq 0 \Rightarrow x \in (\frac{-a-\sqrt{a^2-4Ak}}{2A},\frac{-a+\sqrt{a^2-4Ak}}{2A})$
	- if discriminant $=a^2-4Ak < 0$ the parable is always above $0$, and thus has no solutions. This second case is the equivalent of having negative radius, where we see that it has no volume at all.
- Case $B<0 \land C=0$ or $B=0 \land C<0$:

	Let's say it's $C=0$. We have that the contribute $cz$ spans onto $(-\infty,+\infty)$. This means that for whatever $x$, chosen the "big" enough $cz$ the inequality holds. So any $x$ is acceptable.
	On the other hand, if it's also $c=0$, the case reduces to the previous one, but with $k\equiv-r-\frac{b^2}{4B}$.
- Case $B,C=0$:

	This case rely again on the lower terms. If either $b\neq0$ or $c\neq0$ we have that any $x$ allow a choice of $\tilde{y},\tilde{z}$ that satisfy the inequality.
	If they are $b,c=0$ we have instead $Ax^2+ax+r>0$. This restricts the allowed values to $x\in(\frac{-a-\sqrt{a^2-4Ar}}{2A},\frac{-a+\sqrt{a^2-4Ar}}{2A})$, remembering that $A<0$ by assumption. This is correct only if well defined, so only if $a^2-4Ar>0$(if $=0$ we have a point, that by reducing the dimension of the domain, makes the integral $0$).

Let's now proceed with the assumption $A=0$ (and by also restoring the initial values of the coefficients).
- Case $B>0 \lor C>0$:
	
	If either of the 2 is $>0$, we have that exists a choice of big enough $\tilde{y},\tilde{z}$, such that the left side is greater than $0$. For this reason the max and min are once again $-\infty$ and $+\infty$.
- Case $B,C<0$:

	Rewriting the inequality $ax+By^2+by+Cz^2+cz+r>0$, we see that we have an arbitrarily large negative contribute from the $B$ and $C$. The $ax$ instead spans from $-\infty$ to $+\infty$ regardless of the sign of $a$.
	From this observation we can see that the unbounded part is either $+\infty$ if $a>0$, or $-\infty$ if $a<0$, given a big enough choice of $ax$, and small enough of $y,z$.
	Let's now find the finite bound. We rewrite the inequality as $ax+r>-By^2-by-Cz^2-cz$. We can change notation, calling $B\equiv-B$ and $C\equiv-C$, making them become both $>0$, getting $ax+r>By^2-by+Cz^2-cz$. We can once more complete the squares obtaining $ax+r>(\sqrt{B}y-\frac{b}{2\sqrt{B}})^2-\frac{b^2}{4B}+(\sqrt{C}y-\frac{c}{2\sqrt{C}})^2-\frac{c^2}{4C}$.
	Defining $k\equiv r+\frac{b^2}{4B}+\frac{c^2}{4C}$ we get $ax+k>(\sqrt{B}y-\frac{b}{2\sqrt{B}})^2+(\sqrt{C}y-\frac{c}{2\sqrt{C}})^2$. Let's analyze the two cases for $a$.
	- $a>0$, we have that $x>\frac{1}{a}[(\sqrt{B}y-\frac{b}{2\sqrt{B}})^2+(\sqrt{C}y-\frac{c}{2\sqrt{C}})^2-k]$. Since the squares are positive, and that we want the minimum value of $x$, in function of every $y,z$, we minor the squares by their least value, which is $0$, getting that $x>-\frac{k}{a}$.
	- $a<0$, the same reasoning works, except that this time when dividing by $a$ we have to switch inequality, getting $x<-\frac{k}{a}$.

	A last case must be mentioned, where $a=0$. Under these conditions we have $By^2+by+Cz^2+cz+r>0$, which is either always true for each $x$ if there exists some $\tilde{y},\tilde{z}$ for which it holds, or always false independently from $x$. This check can be done once again with some intuition, seeing that since $B,C<0$ this is an ellipse, which with $B\equiv -B$ and $C\equiv -C$ can be rewritten as $r>By^2-by+Cz^2-cz$. By squaring we get $r>(\sqrt{B}y-\frac{b}{2\sqrt{B}})^2-\frac{b^2}{4B}+(\sqrt{C}y-\frac{c}{2\sqrt{C}})^2-\frac{c^2}{4C}$.
	We define $k\equiv r+\frac{b^2}{4B}+\frac{c^2}{4C}$, and get $(\sqrt{B}y-\frac{b}{2\sqrt{B}})^2+(\sqrt{C}y-\frac{c}{2\sqrt{C}})^2 < k$. If $k < 0$ there exists at least one point that is in the ellipse.
- Case $B=0 \lor C=0$:

	If either one of the two is $0$, let's say $C=0$, it remains the contribute of $cz$, which spans on $(-\infty,+\infty)$. So as long as $c\neq0$ every $x$ is a solution.
	If both $B,C=0$, once again it's the same situation. The only exception is if both $b,c=0$. In this case it remains $ax+r>0$. The solutions are similar to before: if $a>0$ it's $x>-\frac{r}{a}$, else if $a<0$ it's $x<-\frac{r}{a}$, and if $a=0$ it's always true if $r>0$.
	
### Romberg's method + Richardson's Extrapolation
Having established a domain the next step is actually calculating the integral using Romberg's algorithm. It goes as following:
The first column $R_{i,1} \forall i\geq1$ is given by the 3D trapezoidal rule.
The rule is implemented as follows:

![trapezoid method](https://raw.githubusercontent.com/Sonodaart/Triple-Integral-Calculator/main/trapezoid3D.png)

We compute a regular trapezoid rule over the x axis on the red dots. Each red dot is the result of a regular trapezoidal rule over the y axis(the red lines). And each blue(or red) dot is again the result of a regular trapezoidal rule over the z axis(the blue lines). The regular trapezoidal rule is:
$$R_{i,1} = \frac{h}{2}[f(a)+2\sum\limits_{k=1}^{2^{i}-1} f(a+kh)+f(b)]$$
with $h$ the step-size. The reason why the upper limit of the sum is $2^{i}-1$ is because by construction we always halve h each step.

At this point to get the next values in each row we take advantage of Richardson's Extrapolation. This formula cancels the error contributes, making each step more precise. In particular the error of $R_{i,j}$ is of $O(h^{2j})$. The way we cancel the contributes is given by the following relation $R_{i,j}=\frac{4^{j-1}R_{i,j-1}-R_{i-1,j-1}}{4^{j-1}-1}$.
This is thus how we proceed in filling the table of approximations.

![Romberg's method](https://raw.githubusercontent.com/Sonodaart/Triple-Integral-Calculator/main/romberg.png)

To estimate the error we can watch two consecutive values of R, such that $\epsilon \equiv |R_{i,j}-R_{i-1,j-1}|$.
### Extensions of the Solution - Adaptive quadrature
What's been presented is the basic approach. A problematic that arose is that by being in $3$ dimensions, the number of points for each trapezoidal rule grows as a cube. This is really bad if the size of the domain is large enough, since small steps require huge computational amount of time. For this reason is wise to require a good precision only where is needed. This is the idea on which adaptive quadrature works on.
The technique is quite straightforward:
- The integral with it's error is computed
- If the error is below the allowed tolerance the result is found
- If the error is above the allowed tolerance the domain is split into subdomains, and the integration is repeated on every subdomain.

This approach has two benefits. The first is that by splitting the domain the growth of the number of points is no longer cube. The second is that doing this way, all the regions that behave "well" requires "less" computation, since the integral converge faster. And on the other hand, the computation is concentrated on those regions that are hard to approximate.
Of course this technique has also slight disadvantages, one of which is that the Romberg's algorithm loses relevance. In fact we trade the precision given by the Richardson's extrapolation that requires a huge number of points, to the sum of the contributes of different regions, that are instead calculated with less steps of the Romberg's algorithm.
A final note is on how the domain is split.
![inner domain split](https://raw.githubusercontent.com/Sonodaart/Triple-Integral-Calculator/main/innerDomainSplit.png)
Being in 3 dimensions, what's been done is splitting the domain in the 3D-equivalent of bisecting a line. In this way every split creates $2^3=8$ separate subdomains.


