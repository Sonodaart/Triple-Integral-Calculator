
# Triple Integral calculator

##  Table of Contents

- [File Management](#file-management)
- [Usage](#table-of-contents)
- [Underlying theory](#underlying-theory)
	- [Mathematical Formulation of the Problem](#mathematical-formulation-of-the-problem)
	- [Numerical Solution](#numerical-solution)
# File Management
Triple Integral calculator
```
│
├── bin // executable 
│ └── main
├── doc // documentation 
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

# Underlying theory
## Mathematical Formulation of the Problem
The objective of the program is to numerically approximate(with a certain error) the value that would assume the corresponding Lesbegue integration.
We have that the value of the integral is equal to the measure of the volume of the surface. In other words, given a function $f(x,y,z):D\subseteq \mathbb{R}^3 \rightarrow \mathbb{R}^3$, being $D$ a normal regular domain, we have.
$$\iiint_D f(x,y,z)dD = m_{4}(S_f)$$
The function $f$ is provided from the user, and it's supposed that the function has an integral(this means it can also be $\pm\infty$).
The domain $D$ is also given as input, in the form of the intersection of two inequalities of the form $Ax^2+ax+By^2+by+Cz^2+cz+r\gtreqless0$. From this we can see that the border is given by the intersection of two $C^{\infty}(\mathbb{R}^3)$ functions, and thus is also normal and regular.

At this point the aim of the program is to just approximate the value of $m_4(S_f)$(where by $m_4$ it's to be intended the 4D measure function).

It must be also mentioned that the domain can be unbounded, in this case an estimate of the improper integral is given.
## Numerical Solution
The method used to solve the problem relies on romberg integration method. 
### Domain
But let's begin by looking at the domain of integration.
Given that the domain is the intersection of two inequalities of the form $Ax^2+ax+By^2+by+Cz^2+cz+r\gtreqless0$. What we do is find the maximum and minimum value assumed by $x,y$ and $z$. After finding each max and min value we look for common intervals. This way we get a rectangular set that contains the domain $D$.
The only thing to do is calculate the values assumed by each coordinate. Let's do the calculations for $x$, and for the others follows with the same logic.
I'm gonna assume that if the inequality is either $<$ or $\leq$, the signs of the coefficients are changed($A\equiv-A,...$), and also the inequality into either $>$ or $\geq$. With this notation it's possible to tackle both $\gtreqless$ at the same time, having to deal only with the case $>$(or $\geq$. Since they're the same let's assume we have $>$.

- Case $A>0$:

	This case is trivial. Given big enough $x$ the inequality holds. Of course there may be intervals in between for which it doesn't hold, but since we want the max and min we immediately know them to be min$=-\infty$ and max$=+\infty$.

From now on it's gonna be assumed that $A<0$.
- Case $B>0 \lor C>0$:

	If any of the 2 are $>0$, we have a situation similar to before, where for every $x$, there exist a $\tilde{y},\tilde{z}$ such that being big enough brings the left side above $0$.
- Case $B<0 \land C<0$:

	Rearranging the terms and practicing a movement of the origin we see that this is the equation of a generic ellipsoid $\frac{x^2}{a^2}+\frac{y^2}{b^2}+\frac{z}{c'^2}<1$(with here some generic $a,b,c \in \mathbb{R}$).
	We proceed to write $r>-Ax^2-ax-By^2-by-Cz^2-cz$. With this, since we know that $A,B,C<0$ we can change notation once a gain absorbing the signs into the coefficients getting $A\equiv-A,a\equiv-a,...$.
	We end up with $Ax^2+ax+By^2+by+Cz^2+cz<r$. We now complete the squares getting $Ax^2+ax+(\sqrt{B}y+\frac{b}{2\sqrt{B}})^2-\frac{b^2}{4B}+(\sqrt{C}y+\frac{c}{2\sqrt{C}})^2-\frac{c^2}{4C}<r$. Since $B,C>0$ the squares of $y$ and $z$ gives values from $0$ to $+\infty$. Since we want the max range of values for $x$ we minor those terms with $0$, getting $Ax^2+ax-\frac{b^2}{4B}-\frac{c^2}{4C}<r$.
	By calling $k\equiv -r-\frac{b^2}{4B}-\frac{c^2}{4C}$, we can rewrite it as $Ax^2+ax+k<0$. This is a simple second order equation with solutions(remembering that $A>0$):
	- if discriminant$=a^2-4Ak\geq 0 \Rightarrow x \in (\frac{-a-\sqrt{a^2-4Ak}}{2A},\frac{-a+\sqrt{a^2-4Ak}}{2A})$
	- if discriminant$=a^2-4Ak<0$ the parable is always above $0$, and thus has no solutions. This second case is the equivalent of having negative radius, where we see that it has no volume at all.

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
	We define $k\equiv r+\frac{b^2}{4B}+\frac{c^2}{4C}$, and get $\(\sqrt{B}y-\frac{b}{2\sqrt{B}})^2+(\sqrt{C}y-\frac{c}{2\sqrt{C}})^2<k$. If $k\geq0$ there exists at least one point that is in the ellipse.
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

At this point to get the next values in each row we take advantage of Richardson's Extrapolation. This formula cancels the error contributes, making each step more precise. In particular the error of $R_{i,j}$ is of $O(h^{2j})$. The way we cancel the contributes is given by the following relation $R_{i,j}=\frac{4^{j-1}R{i,j-1}-R_{i-1,j-1}}{4^{j-1}-1}$.
This is thus how we proceed in filling the table of approximations.
![Romberg's method](https://raw.githubusercontent.com/Sonodaart/Triple-Integral-Calculator/main/romberg.png)

To estimate the error we can watch two consecutive values of R, such that $\epsilon \equiv |R_{i,j}-R_{i,j-1}|$.





# Synchronization

Synchronization is one of the biggest features of StackEdit. It enables you to synchronize any file in your workspace with other files stored in your **Google Drive**, your **Dropbox** and your **GitHub** accounts. This 

- The workspace synchronization will sync all your files, folders and settings automatically. This will allow you to fetch your workspace on any other device.
	> To start syncing your workspace, just sign in with Google in the menu.

- The file synchronization will keep one file of the workspace synced with one or multiple files in **Google Drive**, **Dropbox** or **GitHub**.
	> Before starting to sync files, you must link an account in the **Synchronize** sub-menu.


