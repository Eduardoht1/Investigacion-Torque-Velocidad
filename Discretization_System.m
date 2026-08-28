clc 
clear
close all

%Functions
function print_matrix_name(M)
    matName = inputname(1); % Gets the name of the passed variable
    disp([matName, ' =']);
    disp(M);
end

%Caracterization parameters
R = 12.76; % Armature resistance Ohms
L = 0.019254; %Armature inductance H
Va = 30; % Armature Voltage V
J = 0.001047; % Rotor moment of inertia Kg m^2
b = 0.000737; % Viscous friction coefficient
Kt = 0.436822; % Torque constant
Kb = 0.436822; % Back electromotive force constant

%System Matrices
A=[-b/J Kt/J;-Kb/L -R/L];
B=[0; 1/L];
C=[1 0];
D=0;
T=0.001;

%Continuous
sys = ss(A, B, C, D);

%Forward Euler Discretization [s = (z - 1) / T]
I = eye(size(A)); 
A_fe = I + A*T;
B_fe = B*T;
C_fe = C;
D_fe = D;
sys_fe = ss(A_fe, B_fe, C_fe, D_fe, T);

%Forward Euler Matrices
print_matrix_name(A_fe)
print_matrix_name(B_fe)
print_matrix_name(C_fe)
print_matrix_name(D_fe)

%Controllability
Ctrl=[B_fe A_fe*B_fe];
rank(Ctrl) % It has to be 2 to bew controllable
