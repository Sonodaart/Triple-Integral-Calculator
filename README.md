# Triple Integral calculator

##  Table of Contents

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
<div class="katex">
$ \iiint_D f(x,y,z)dD = m_{4}(S_f) $
</div>
The function $f$ is provided from the user, and it's supposed that function has an integral(this means it can also be $\pm\infty$).
The domain $D$ is also given as input, in the form of the intersection of two inequalities of the form $Ax^2+ax+By^2+by+Cz^2+cz+r\gtreqless0$. From this we can see that the border is given by the intersection of two $C^{\infty}(\mathbb{R}^3)$ functions, and thus is also normal and regular.

At this point the aim of the program is to just approximate the value of $m_4(S_f)$(where by $m_4$ it's to be intended the 4D measure function).

It must be also mentioned that the domain can be unbounded, in this case an estimate of the improper integral is given.
## Numerical Solution
The method used to solve the problem relies on romberg integration method. 
### Domain
But let's begin by looking at the domain of integration.
Given that the domain is the intersection of two inequalities of the form $Ax^2+ax+By^2+by+Cz^2+cz+r\gtreqless0$. What we do is find the maximum and minimum value assumed by $x,y$ and $z$. After finding each max and min value we look for common intervals. This way we get a rectangular set that contains the domain $D$.
The only thing to do is estimating the values assumed by each coordinate. Let's do the calculations for $x$, and for the others follows with the same logic.

The first thing we want to do is rewriting the inequality as $Ax^2+ax+(\sqrt{B}y+\frac{b}{2\sqrt{B}})^2-\frac{b^2}{4B}+(\sqrt{C}y+\frac{c}{2\sqrt{C}})^2-\frac{c^2}{4C}+r\gtreqless0$ by completing the squares. We can see that with a proper choice of $y$ and $z$ we can make their squares $0$. In addition we can also impose the inequality to be $>$ or $\geq$ by changing accordingly the signs of all the coefficients(le'ts say it's $>$).
So, evaluating the inequality at such $\tilde{y},\tilde{z}$ we get $Ax^2+ax-\frac{b^2}{4B}-\frac{c^2}{4C}+r>0$. We define $k \equiv -\frac{b^2}{4B}-\frac{c^2}{4C}+r$, such that we have $Ax^2+ax+k>0$.
At this point we solve for x, getting $x=\frac{-a\pm\sqrt{a^2-4Ak}}{2A}$. By studying the discriminant and the sign of A we can get different solutions:
- $A>0$
	- $a^2-4Ak\geq 0 \Rightarrow x \in (\frac{-a-\sqrt{a^2-4Ak}}{2A},\frac{-a+\sqrt{a^2-4Ak}}{2A})$
	- $a^2-4Ak< 0 \Rightarrow x \in (-\infty,\frac{-a-\sqrt{a^2-4Ak}}{2A}) \cup (\frac{-a+\sqrt{a^2-4Ak}}{2A},+\infty)$, but since we want the max and min we get $(-\infty,+\infty)$
- $A<0$
	- $a^2-4Ak\geq 0 \Rightarrow x \in (-\infty,+\infty)$
	- $a^2-4Ak< 0 \Rightarrow x \in (\frac{-a-\sqrt{a^2-4Ak}}{2A},\frac{-a+\sqrt{a^2-4Ak}}{2A})$

### Romberg's method + Richardson's Extrapolation
Having established a domain the next step is actually calculating the integral using Romberg's algorithm. It goes as following:
The first column $R_{i,1} \forall i\geq1$ is given by the 3D trapezoidal rule.
The rule is implemented as follows:
![trapezoid method](https://raw.githubusercontent.com/Sonodaart/Triple-Integral-Calculator/main/trapezoid3D.png)

We compute a regular trapezoid rule over the x axis on the red dots. Each red dot is the result of a regular trapezoidal rule over the y axis(the red lines). And each blue(or red) dot is again the result of a regular trapezoidal rule over the z axis(the blue lines). The regular trapezoidal rule is:

<div class="katex">
$ R_{i,1} = \frac{h}{2}[f(a)+2\sum\limits_{k=1}^{2^{i}-1} f(a+kh)+f(b)] $
</div>
with $h$ the step-size. The reason why the upper limit of the sum is $2^{i}-1$ is because by construction we always halve h each step.

At this point to get the next values in each row we take advantage of Richardson's Extrapolation. This formula cancels the error contributes, making each step more precise. In particular the error of $R_{i,j}$ is of $O(h^{2j})$. The way we cancel the contributes is given by the following relation $R_{i,j}=\frac{4^{j-1}R{i,j-1}-R_{i-1,j-1}}{4^{j-1}-1}$.
This is thus how we proceed in filling the table of approximations.
![Romberg's method](https://raw.githubusercontent.com/Sonodaart/Triple-Integral-Calculator/main/romberg.png)






# Synchronization

Synchronization is one of the biggest features of StackEdit. It enables you to synchronize any file in your workspace with other files stored in your **Google Drive**, your **Dropbox** and your **GitHub** accounts. This 

- The workspace synchronization will sync all your files, folders and settings automatically. This will allow you to fetch your workspace on any other device.
	> To start syncing your workspace, just sign in with Google in the menu.

- The file synchronization will keep one file of the workspace synced with one or multiple files in **Google Drive**, **Dropbox** or **GitHub**.
	> Before starting to sync files, you must link an account in the **Synchronize** sub-menu.

###  StarterKit Files

```



```


## Publish a File

You can publish your file by opening the **Publish** sub-menu and by clicking **Publish to**. For some locations, you can choose between the following formats:

- Markdown: publish the Markdown text on a website that can interpret it (**GitHub** for instance),
- HTML: publish the file converted to HTML via a Handlebars template (on a blog for example).

## Update a publication

After publishing, StackEdit keeps your file linked to that publication which makes it easy for you to re-publish it. Once you have modified your file and you want to update your publication, click on the **Publish now** button in the navigation bar.

> **Note:** The **Publish now** button is disabled if your file has not been published yet.



# Markdown extensions

StackEdit extends the standard Markdown syntax by adding extra **Markdown extensions**, providing you with some nice features.

> **ProTip:** You can disable any **Markdown extension** in the **File properties** dialog.


## SmartyPants

SmartyPants converts ASCII punctuation characters into "smart" typographic punctuation HTML entities. For example:

|                |ASCII                          |HTML                         |
|----------------|-------------------------------|-----------------------------|
|Single backticks|`'Isn't this fun?'`            |'Isn't this fun?'            |
|Quotes          |`"Isn't this fun?"`            |"Isn't this fun?"            |
|Dashes          |`-- is en-dash, --- is em-dash`|-- is en-dash, --- is em-dash|


## KaTeX

You can render LaTeX mathematical expressions using [KaTeX](https://khan.github.io/KaTeX/):

The *Gamma function* satisfying $\Gamma(n) = (n-1)!\quad\forall n\in\mathbb N$ is via the Euler integral

$$
\Gamma(z) = \int_0^\infty t^{z-1}e^{-t}dt\,.
$$

> You can find more information about **LaTeX** mathematical expressions [here](http://meta.math.stackexchange.com/questions/5020/mathjax-basic-tutorial-and-quick-reference).


## UML diagrams

You can render UML diagrams using [Mermaid](https://mermaidjs.github.io/). For example, this will produce a sequence diagram:

```mermaid
sequenceDiagram
Alice ->> Bob: Hello Bob, how are you?
Bob-->>John: How about you John?
Bob--x Alice: I am good thanks!
Bob-x John: I am good thanks!
Note right of John: Bob thinks a long<br/>long time, so long<br/>that the text does<br/>not fit on a row.

Bob-->Alice: Checking with John...
Alice->John: Yes... John, how are you?
```

And this will produce a flow chart:

```mermaid
graph LR
A[Square Rect] -- Link text --> B((Circle))
A --> C(Round Rect)
B --> D{Rhombus}
C --> D
```
