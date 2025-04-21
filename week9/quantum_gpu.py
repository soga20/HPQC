import torch as pt
import numpy as np
import time

pt.autograd.set_grad_enabled(False)  # Disable autograd for performance

if pt.cuda.is_available():
    print("GPU available")
else:
    print("Sorry, only CPU available")

# Global workspace and qubit name stack
workspace = pt.tensor([[1.]], device='cpu', dtype=pt.float32)
namestack = []

def pushQubit(name, weights):
    global workspace, namestack
    if workspace.shape == (1, 1):
        namestack = []  # Reset if workspace is empty
    namestack.append(name)
    weights = np.array(weights) / np.linalg.norm(weights)  # Normalize
    weights = pt.tensor(weights, device=workspace.device, dtype=workspace.dtype)
    workspace = pt.reshape(workspace, (1, -1))
    workspace = pt.kron(workspace, weights)

def tosQubit(name):
    global workspace, namestack
    k = len(namestack) - namestack.index(name)
    if k > 1:
        namestack.append(namestack.pop(-k))
        workspace = pt.reshape(workspace, (-1, 2, 2**(k - 1)))
        workspace = pt.swapaxes(workspace, -2, -1)
        workspace = pt.reshape(workspace, (1, -1))

def applyGate(gate, *names):
    global workspace
    if list(names) != namestack[-len(names):]:
        for name in names:
            tosQubit(name)
    workspace = pt.reshape(workspace, (-1, 2**len(names)))
    subworkspace = workspace[:, -gate.shape[0]:]
    gate = pt.tensor(gate.T, device=workspace.device, dtype=workspace.dtype)
    if workspace.device.type == 'cuda':
        pt.matmul(subworkspace, gate, out=subworkspace)
    else:
        subworkspace[:, :] = pt.matmul(subworkspace, gate)

def probQubit(name):
    global workspace
    tosQubit(name)
    workspace_reshaped = pt.reshape(workspace, (-1, 2))
    prob = pt.linalg.norm(workspace_reshaped, axis=0)**2
    prob = pt.Tensor.cpu(prob).numpy()
    return prob / prob.sum()

def measureQubit(name):
    global workspace, namestack
    prob = probQubit(name)
    measurement = np.random.choice(2, p=prob)
    workspace = workspace[:, [measurement]] / np.sqrt(prob[measurement])
    namestack.pop()
    return str(measurement)

# --- Gates ---
H_gate = (1/np.sqrt(2)) * np.array([[1, 1], [1, -1]])
Z_gate = np.array([[1, 0], [0, -1]])

def sample_phaseOracle(qubits):
    applyGate(np.array([[0, 1], [1, 0]]), qubits[1])  # X gate
    applyGate(Z_gate, *namestack)
    applyGate(np.array([[0, 1], [1, 0]]), qubits[1])  # Undo X

def zero_phaseOracle(qubits):
    for qubit in qubits:
        applyGate(np.array([[0, 1], [1, 0]]), qubit)  # X
    applyGate(Z_gate, *namestack)
    for qubit in qubits:
        applyGate(np.array([[0, 1], [1, 0]]), qubit)  # Undo X

def groverSearch(n, printProb=True):
    global workspace, namestack
    namestack = []
    workspace = pt.tensor([[1.]], device=workspace.device, dtype=pt.float32)

    optimalTurns = int(np.pi / 4 * np.sqrt(2**n) - 0.5)
    qubits = [f"q{i}" for i in range(n)]
    for q in qubits:
        pushQubit(q, [1, 1])  # Equal superposition

    for _ in range(optimalTurns):
        sample_phaseOracle(qubits)
        for q in qubits:
            applyGate(H_gate, q)
        zero_phaseOracle(qubits)
        for q in qubits:
            applyGate(H_gate, q)
        if printProb:
            print(probQubit(qubits[0]))

    for q in reversed(qubits):
        print(measureQubit(q), end="")
    print()

# --- Timing comparison ---
# GPU
if pt.cuda.is_available():
    workspace = pt.tensor([[1.]], device='cuda', dtype=pt.float32)
    t = time.process_time()
    groverSearch(6, printProb=False)
    print("With GPU:", time.process_time() - t, "s")

# CPU
workspace = pt.tensor([[1.]], device='cpu', dtype=pt.float32)
t = time.process_time()
groverSearch(6, printProb=False)
print("With CPU (single-core):", time.process_time() - t, "s")
