# Naval Battle

## Summary:

1. The 3D engine
    1. Global idea
2. The physics engine
    1. Rigid body dynamics
    2. Water simulation
    3. Water and rigid body contacts
    4. Damaging rigid bodies
3. Gameplay spécifications
  
## 1. The 3D engine

The 3D engine is written in C++, using openGL. The main goal of this part is to understand how openGL work. In fact, the course we got hasn't got enough in depth for me.

### 1.1 Global idea


## 2. The physics engine

The physics engine is really important for the gameplay. The goal is to have something complex, with plenty of possibilities, but intuitive. In this chapter, we will 
describe how it works.

### 2.1 Rigid Body Dynamics

A Rigid Body is a body without deformations.

#### 2.1.1 Informations needed to describe a rigid body
It has a position (a 3D vector), describing the position of his center of mass.
To describe his movement, it has a speed (a 3D vector) and a torque for each axis (a 3D vector).
It has a mass (a scalar), and a mass moment of inertia (we will call it inertia) (a 3x3 matrix)

#### 2.1.2 Applying a force on a rigid body
We have a rigid body $A$ with a center of mass $c_A$. We want to put a force $f$ on the point $p_A$ , and we want to know what is his new acceleration, and his new 
rotational acceleration.  
If $c_A - p_A$ and $f$ are aligned, we don't have any rotation. So applying the force is pretty straitforward.  
The new $acceleration$ is $acceleration$ += $\frac f {mass}$  
If $c_A - p_A$ and $f$ are not aligned, we need to add some rotational speed too.  
$torque$ += $I^{-1} \cdot ((p_A - c_A) \times f)$  
(" $\times$ " is for the cross product, and " $\cdot$ " is for the matrix-vector multiplication)  

**N.B.:** $I^{-1}$ **should be calculated only once, same for** $\frac 1 {mass}$

#### 2.1.3 Applying an impulse on a rigid body
It follows the same ideas. The unique difference is that we don't change the acceleration, but the speed.

#### 2.1.4 Resolve collisions
This is one of the hardest part.  
We have two rigid bodies colliding: A and B. We suppose they are colliding only in a point or a line. Their velocities before the contact are $v_A$ and $v_B$. $v$ is 
the relative velocity, i.e. $v_B - v_A$. the contact point is $p$. If they are colliding in a line, we will compute this twice, the first time for an extremity of the 
line, and the second time for the other extremity. $n$ is the normal of the contact. $e$ is the coefficiant of restitution (between 0 and 1). $r_A$ is equal to 
$p - c_A$, and $r_B$ is equal to $p - c_B$. Finally, let $R_A = (f - \frac {f \cdot r_A} {\|r_A\|^2} r_A)$.resize($\|r_A\|$). $R_A$ is the projection of $f$ on the 
orthogonal plane of $r_A$, with the size of $r_A$.

We want to know the velocities after the impact.  

$$j = \frac {-(1+e) v \cdot n} {n \cdot n (\frac 1 {m_A} + \frac 1 {m_B}) + I_A^{-1} \cdot (R_A \cdot n)^2 + I_B^{-1} \cdot (R_B \cdot n)^2}$$  

Then, we can change the velocities of the bodies using the impulses:  
$v_A$ += $\frac j {m_A} n$  
$t_A$ += $I_A ^{-1} (R_A \cdot j n)$  
$v_B$ -= $\frac j {m_B} n$  
$t_B$ -= $I_B ^{-1} (R_B \cdot j n)$  
($t_A$ is the torque and $v_A$ is the linear velocity.)  

For further reading: https://www.chrishecker.com/Rigid_Body_Dynamics.
