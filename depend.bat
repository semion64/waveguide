gnuplot -e "file1='data/remeasure/depend/Cv_comp_R_without'" -e "file2='data/measure/depend/Cv_comp_R_without'" plot.plt 
gnuplot -e "file1='data/remeasure/depend/Cv_comp_R_in'" -e "file2='data/measure/depend/Cv_comp_R_in'" plot.plt
gnuplot -e "file1='data/remeasure/depend/Cv_comp_dR'" -e "file2='data/measure/depend/Cv_comp_dR'" plot.plt
gnuplot -e "file1='data/remeasure/depend/Cv_comp_F_without'" -e "file2='data/measure/depend/Cv_comp_F_without'" plot.plt	


plot "data/remeasure/depend/Cv_comp_R_without" with lines,  "data/measure/depend/Cv_comp_R_without" with lines
plot "data/remeasure/depend/Cv_comp_R_in" with lines,  "data/measure/depend/Cv_comp_R_in" with lines
plot "data/remeasure/depend/Cv_comp_dR" with lines,  "data/measure/depend/Cv_comp_dR" with lines
plot "data/remeasure/depend/Cv_comp_F_without" with lines,  "data/measure/depend/Cv_comp_F_without" with lines
plot "data/remeasure/depend/Cv_comp_F_in" with lines,  "data/measure/depend/Cv_comp_F_in" with lines
plot "data/remeasure/depend/Cv_comp_dF" with lines,  "data/measure/depend/Cv_comp_dF" with lines

plot "data/remeasure/depend/Cv_cnt_R_without" with lines,  "data/measure/depend/Cv_cnt_R_without" with lines
plot "data/remeasure/depend/Cv_cnt_R_in" with lines,  "data/measure/depend/Cv_cnt_R_in" with lines
plot "data/remeasure/depend/Cv_cnt_dR" with lines,  "data/measure/depend/Cv_cnt_dR" with lines
plot "data/remeasure/depend/Cv_cnt_F_without" with lines,  "data/measure/depend/Cv_cnt_F_without" with lines
plot "data/remeasure/depend/Cv_cnt_F_in" with lines,  "data/measure/depend/Cv_cnt_F_in" with lines
plot "data/remeasure/depend/Cv_cnt_dF" with lines,  "data/measure/depend/Cv_cnt_dF" with lines

plot "data/remeasure/depend/B_R_without" with lines,  "data/measure/depend/B_R_without" with lines
plot "data/remeasure/depend/B_R_in" with lines,  "data/measure/depend/B_R_in" with lines
plot "data/remeasure/depend/B_dR" with lines,  "data/measure/depend/B_dR" with lines
plot "data/remeasure/depend/B_F_without" with lines,  "data/measure/depend/B_F_without" with lines
plot "data/remeasure/depend/B_F_in" with lines,  "data/measure/depend/B_F_in" with lines
plot "data/remeasure/depend/B_dF" with lines,  "data/measure/depend/B_dF" with lines
