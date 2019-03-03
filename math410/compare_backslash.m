% % Runs trials to reverse engineer factorizations used by backslash\mldivide.

%% Initialization.

% dimension of problem.
n = 2000;

% number of trials.
nt = 25;

% perturbation parameter.
beta = 1e-15;

% Run-time matrix. Columns store run-times for each perturbation. Rows for
% each trial.
times = zeros(nt, 6);

% Make standard basis vectors.
e1 = zeros(n,1); e1(1) = 1;
e2 = zeros(n,1); e2(2) = 1;

% Initialize/allocate solution variable x.
x = zeros(n,1);

% Set up results table.
fprintf('++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n')
fprintf('i  \t || A \t\t|| A1 \t\t|| A2 \t\t|| A3  \t\t|| A4 \t\t|| A5      \n')
fprintf('++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n')

%% Timing experiment.
for i = 1:nt
    
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    % Make A, b to form unperturbed problem.
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
    
    A = diag(1 + rand(n, 1));
    b = rand(n, 1);
    
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    % Solve using \.
    % Use tic/toc to record run-time as times(i,1).
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
    tic
    
    x = A \ b;
    
    times(i, 1) = toc;
    
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    % A1: Perturb by E1 = e1e2'
    % Solve A1*x = b and record run-time as times(i,2).
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    tic
    
    E1 = e1*e2';
    A1 = A + beta * E1;
    
    x = A1 \ b;
    
    times(i, 2) = toc;
    
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    % A2: Perturb by E2 = e1e2' + e2'e1.
    % Solve A2*x = b and record run-time as times(i,3).
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    tic
    
    E2 = E1 + E1';
    A2 = A + beta * E2;
    x = A2 \ b;
    
    times(i, 3) = toc;
    
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    % A3: Perturb by upper triangular matrix E3.
    % Solve A3*x = b and record run-time as times(i,4).
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    tic
    
    E3 = triu(rand(n));
    A3 = A + beta * E3;
    x = A3 \ b;
    
    times(i, 4) = toc;
    
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    % A4: Perturb by Hermitian positive definite matrix E4. 
    % Solve A4*x = b and record run-time as times(i,5).
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
    
    tic
    
    E4 = rand(n);
    E4 = E4 * E4';
    A4 = A + beta * E4;
    x = A4 \ b;
    
    times(i, 5) = toc;
    
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    % A5: Perturb by unstructured matrix E5. 
    % Solve A5*x = b and record run-time as times(i,6).
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    tic
    
    E5 = rand(n);
    A5 = A + beta * E5;
    x = A5 \ b;
    
    times(i, 6) = toc;
    
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    % Print iteration statistics.
    %++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
    fprintf('%3d \t || %1.4e \t|| %1.4e \t|| %1.4e \t|| %1.4e \t|| %1.4e \t|| %1.4e  \n', i, times(i,1), times(i,2), times(i,3), times(i,4), times(i,5), times(i,6));
    
end

%% Summary Statistics.

% Compute mean and standard deviation.
avg = mean(times);
dev = std(times);

% Print run-times table.
fprintf('++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n')
fprintf('avg \t || %1.4e \t|| %1.4e \t|| %1.4e \t|| %1.4e \t|| %1.4e \t|| %1.4e  \n', avg(1), avg(2), avg(3), avg(4), avg(5), avg(6));
fprintf('std \t || %1.4e \t|| %1.4e \t|| %1.4e \t|| %1.4e \t|| %1.4e \t|| %1.4e \n', dev(1), dev(2), dev(3), dev(4), dev(5), dev(6));
fprintf('++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n')