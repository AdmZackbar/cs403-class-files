% % Compares cholesky factorization (using mldivide) and matrix inverse.

%% Initialize input and result vectors. 

% Problem sizes.
nvect = [50; 100; 250; 500; 1000; 1500; 2000; 2500];
nn = length(nvect);

% Number of repeated trials.
nt = 20;


% vectors to storage average error and computation times.
avtime = zeros(nn,1); % time for original version.
avtimechol = zeros(nn,1); % time for version specialized for tridiagonal 
averr = zeros(nn,1); % average norm difference in R.

%% Compare system solving algorithms.

fprintf('++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n')
fprintf('n \t || Backslash \t || Cholesky \t || Err \t \n')
fprintf('++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n')

for i = 1:nn % Cycle through problem sizes.
    
    % Extract problem size.
    n = nvect(i); 
    
    for t = 1:nt % Repeat nt times.
        
        %+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
        % Make n by n random positive definite matrix A and n-dim b.
        %+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
        
        A = rand(n);
        A = A * A';
        b = rand(n, 1);
        
        %+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++        
        % Solve using \.        
        % Add run-time to avtime(t) (will average total at end).
        %+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
           
        tic
        
        x = A \ b;
        
        avtime(i) = toc + avtime(i);
        
        %+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++        
        % Solve using Cholesky.
        % Add run-time to avtimechol(t) (will average total at end).
        %+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++        
        
        tic
        
        R = chol(A);
        optsY.LT = true;
        y = linsolve(R', b, optsY);
        optsX.UT = true;
        xc = linsolve(R, y, optsX);
        
        avtimechol(i) = toc + avtimechol(i);
        
        % Update relative error.
        averr(i) = averr(i) + norm(x - xc);       
        
    end
    
    % Average timing and error counts.
    avtime(i) = avtime(i)/nt;
    avtimechol(i) = avtimechol(i)/nt;
    averr(i) = averr(i)/nt;
   
    % Displays average computation times and errors.
    fprintf('%3d \t || %1.3e \t || %1.3e \t || %1.3e \n', nvect(i), avtime(i), avtimechol(i), averr(i))
end
        

%% Plot.

% Initialize plot.
figure; hold('on')

set(gca,'XScale','log','YScale','log');

% Plot mean computation time for classical Cholesky factorization.
loglog(nvect, avtime, 'r+--', 'LineWidth', 2.25, 'MarkerSize', 10); 

% Plot mean computation time for column-oriented version.
loglog(nvect, avtimechol, 'bo-', 'LineWidth', 2.25, 'MarkerSize', 10)

% Label plot.
legend ('Backslash', 'Cholesky', 'Location','northwest')
xlabel ('Number of rows/columns')
ylabel('Time')
title('Average computation time for system solution')
set(gca, 'FontSize', 18)
hold('off')

