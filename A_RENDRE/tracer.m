M1 = load("-ascii", "Data_Genetique.txt");
M2 = load("-ascii", "calculated_points.txt");
subplot(2,1,1)
plot(M1(:,1), M1(:,2), M2(:,1), M2(:,2), "LineWidth", 1)