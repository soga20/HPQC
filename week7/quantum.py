import numpy as np

def pushQubit(weights):
    global workspace
    workspace = np.reshape(workspace,(1,-1)) 
    workspace = np.kron(workspace,weights)

def applyGate(gate):
    global workspace
    workspace = np.reshape(workspace,(-1,gate.shape[0]))     
    np.matmul(workspace,gate.T,out=workspace)

X_gate = np.array([[0, 1],   # Pauli X gate 
                  [1, 0]])   # = NOT gate

workspace = np.array([[1.]])       # reset workspace 
pushQubit([1,0])
print("input",workspace)
applyGate(X_gate)                  # = NOT 
print("output",workspace)
