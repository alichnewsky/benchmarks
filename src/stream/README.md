OpenMP version of STREAM from NERSC: 
http://www.nersc.gov/users/computational-systems/cori/nersc-8-procurement/trinity-nersc-8-rfp/nersc-8-trinity-benchmarks/stream/

Download original code @
http://www.nersc.gov/assets/Trinity--NERSC-8-RFP/Benchmarks/Jan9/stream.tar

Hard-coded use of 480 MB of memory to do stream-triad.
You'd need to change that value if you want to run on machines where this would fit into caches...

provided you save various results as 
results/README.stream.triad.MACHINE_NAME

python stream-plots.py results/README.stream.triad.* 
would generate a plot named bandwidths.png
