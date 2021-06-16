rm trial.log
./ss >& trial.log
cd AMPL/
rm ILP.log LP.log
ampl sum.run >& ILP.log
ampl sumLP.run >& LP.log
cd ..
grep -rni "with difference" trial.log >& result.log
