M1 = load("-ascii", "Data_Genetique.txt");
M2 = load("-ascii", "calculated_points.txt");
M3 = load("-ascii", "B_egal_cent.txt");

subplot(2,1,1)
plot(M1(:,1), M1(:,2), M2(:,1), M2(:,2), "LineWidth", 1)

subplot(2,1,2)
plot(M1(:,1), M1(:,2), M3(:,1), M3(:,2), "LineWidth", 1)