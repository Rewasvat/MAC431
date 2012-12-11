#copy folder to /opt/AMDAPP/samples/opencl/cl/app/MatrixShift
echo "Deploying Application..."
sudo cp -r ./MatrixShift/ /opt/AMDAPP/samples/opencl/cl/app/
cd /opt/AMDAPP/samples/opencl/cl/app/MatrixShift
sudo make

#execute from /opt/AMDAPP/samples/opencl/bin/x86
echo
echo "Executing..."
/opt/AMDAPP/samples/opencl/bin/x86/MatrixShift $1 $2 $3
