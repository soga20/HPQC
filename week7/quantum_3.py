import numpy as np

def pushQubit(weights):
    global workspace
    workspace = np.reshape(workspace,(1,-1)) 
    workspace = np.kron(workspace,weights)

def applyGate(gate):
    global workspace
    workspace = np.reshape(workspace,(-1,gate.shape[0]))     
    np.matmul(workspace,gate.T,out=workspace)


def tosQubit(k):
    global workspace
    if k > 1:                                               # if non-trivial
        workspace = np.reshape(workspace,(-1,2,2**(k-1)))
        workspace = np.swapaxes(workspace,-2,-1)

def probQubit():
    global workspace
    workspace = np.reshape(workspace,(-1,2)) 
    return np.linalg.norm(workspace,axis=0)**2
def measureQubit():
    global workspace
    prob = probQubit()
    measurement = np.random.choice(2,p=prob)         # select 0 or 1 
    workspace = (workspace[:,[measurement]]/
    np.sqrt(prob[measurement])) 
    return str(measurement)


H_gate = np.array([[1, 1],                      # Hadamard gate 
                   [1,-1]]) * np.sqrt(1/2)
TOFF_gate = np.array([[1, 0, 0, 0, 0, 0, 0, 0], # Toffoli gate
                     [0, 1, 0, 0, 0, 0, 0, 0],
                     [0, 0, 1, 0, 0, 0, 0, 0],
                     [0, 0, 0, 1, 0, 0, 0, 0],
                     [0, 0, 0, 0, 1, 0, 0, 0],
                     [0, 0, 0, 0, 0, 1, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0, 1],
                     [0, 0, 0, 0, 0, 0, 1, 0]])

workspace = np.array([[1.]]) 
for i in range(16):
    pushQubit([1,0])                      # push a zero qubit
    applyGate(H_gate)                     # set equal 0 and 1 probability
    pushQubit([1,0])                      # push a 2nd zero qubit
    applyGate(H_gate)                     # set equal 0 and 1 probability
    pushQubit([1,0])                      # push a dummy zero qubit
    applyGate(TOFF_gate)                  # compute Q3 = Q1 AND Q2
    q3 = measureQubit()                   # pop qubit 3
    q2 = measureQubit()                   # pop qubit 2
    q1 = measureQubit()                   # pop qubit 1
    print(q1+q2+q3,end=",")

