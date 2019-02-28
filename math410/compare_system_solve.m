% % Compares LU factorization (using mldivide) and matrix inverse.

%% Initialize input and result vectors. 

% Problem sizes.
nvect = [50; 100; 250; 500; 1000; 1500; 2000; 2500];
nn = length(nvect);

% Number of repeated trials.
nt = 20;


% vectors to storage average error and computation times.
avtime = zeros(nn,1); % time for original version.
avtimeinv = zeros(nn,1); % time for version specialized for tridiagonal 
averr = zeros(nn,1); % average norm difference in R.

%% Compare system solving algorithms.

fprintf('++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n')
fprintf('n \t\t || LU \t\t\t || Inverse \t || Err \t \n')
fprintf('++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n')

for i = 1:nn % Cycle through problem sizes.
    
    % Extract problem size.
    n = nvect(i); 
    
    for t = 1:nt % Repeat nt times.
        
        % Make n by n random matrix A.              
        % Make right-hand side vector b.        
        
        A = rand(n);
        b = rand(n, 1);
        
        %+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++        
        % Solve using LU factorization.
        % Save solution as xlu.
        %+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        tic % start timer.         
        
        xlu = A \ b;
        
        tmp = toc; % stop timer.
        
        % Update total time.
        avtime(i) = avtime(i) + tmp;     
        
        %+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++        
        % Solve using matrix inverse.
        % Save solution as x.
        %+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++        
        tic
        
        x = inv(A) * b;
        
        tmp = toc;
        
        % Update timing measurements.
        avtimeinv(i) = avtimeinv(i) + tmp;
        
        % Update relative error.
        averr(i) = averr(i) + norm(x - xlu);       
        
    end
    
    % Average timing and error counts.
    avtime(i) = avtime(i)/nt;
    avtimeinv(i) = avtimeinv(i)/nt;
    averr(i) = averr(i)/nt;
   
    % Displays average computation times and errors.
    fprintf('%3d \t || %1.3e \t || %1.3e \t || %1.3e \n', nvect(i), avtime(i), avtimeinv(i), averr(i))
end
        

%% Plot.

% Initialize plot.
figure; hold('on')

set(gca,'XScale','log','YScale','log');

% Plot mean computation time for classical Cholesky factorization.
loglog(nvect, avtime, 'r+--', 'LineWidth', 2.25, 'MarkerSize', 10); 

% Plot mean computation time for column-oriented version.
loglog(nvect, avtimeinv, 'bo-', 'LineWidth', 2.25, 'MarkerSize', 10)

% Label plot.
legend ('LU', 'Inverse', 'Location','northwest')
xlabel ('Number of rows/columns')
ylabel('Time')
title('Average computation time for system solution')
set(gca, 'FontSize', 18)
hold('off')

