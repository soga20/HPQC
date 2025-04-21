import numpy as np

# Global workspace and name stack
workspace = np.array([[1]], dtype=np.single)
namestack = []

def pushQubit(name, weights):
    global workspace, namestack
    if workspace.shape == (1, 1):  # if workspace is empty
        namestack = []            # reset stack
    namestack.append(name)
    weights = weights / np.linalg.norm(weights)  # normalize
    weights = np.array(weights, dtype=workspace[0, 0].dtype)
    workspace = np.reshape(workspace, (1, -1))
    workspace = np.kron(workspace, weights)

def applyGate(gate, *names):
    global workspace
    for name in names:
        tosQubit(name)
        workspace = np.reshape(workspace, (-1, gate.shape[0]))
        np.matmul(workspace, gate.T, out=workspace)

def tosQubit(name):  # simulate move-to-top (no-op for now)
    pass  # Placeholder – actual swapping logic would go here

def measureQubit(name):
    # For simplicity, randomly collapse based on probability
    tosQubit(name)
    prob = probQubit(name)
    bit = "1" if np.random.rand() < prob else "0"
    return bit

def probQubit(name):
    tosQubit(name)
    workspace_reshaped = np.reshape(workspace, (-1, 2))
    probs = np.sum(np.abs(workspace_reshaped)**2, axis=0)
    return probs[1]  # Probability of |1⟩

# Quantum gates
X_gate = np.array([[0, 1], [1, 0]])      # Pauli-X
Z_gate = np.array([[1, 0], [0, -1]])     # Pauli-Z
H_gate = (1/np.sqrt(2)) * np.array([[1, 1], [1, -1]])

CNOT_gate = np.array([
    [1, 0, 0, 0],
    [0, 1, 0, 0],
    [0, 0, 0, 1],
    [0, 0, 1, 0]
])

TOFF_gate = np.eye(8)
TOFF_gate[[6, 7], [6, 7]] = 0
TOFF_gate[6, 7] = 1
TOFF_gate[7, 6] = 1

def TOFFn_gate(ctl, result):
    n = len(ctl)
    if n == 0:
        applyGate(X_gate, result)
    elif n == 1:
        applyGate(CNOT_gate, ctl[0], result)
    elif n == 2:
        applyGate(TOFF_gate, ctl[0], ctl[1], result)
    elif n > 2:
        k = 0
        while f"temp{k}" in namestack:
            k += 1
        temp = f"temp{k}"
        pushQubit(temp, [1, 0])
        applyGate(TOFF_gate, ctl[0], ctl[1], temp)
        TOFFn_gate(ctl[2:] + [temp], result)
        applyGate(TOFF_gate, ctl[0], ctl[1], temp)
        measureQubit(temp)

def zero_booleanOracle(qubits, result):
    for qubit in qubits:
        applyGate(X_gate, qubit)
    TOFFn_gate(qubits, result)
    for qubit in qubits:
        applyGate(X_gate, qubit)

def zero_phaseOracle(qubits):
    for qubit in qubits:
        applyGate(X_gate, qubit)
    applyGate(Z_gate, *namestack)
    for qubit in qubits:
        applyGate(X_gate, qubit)

def sample_phaseOracle(qubits):
    applyGate(X_gate, qubits[1])
    applyGate(Z_gate, *namestack)
    applyGate(X_gate, qubits[1])

def groverSearch(n, printProb=True):
    global workspace
    workspace = np.array([[1.]], dtype=np.single)
    qubits = [f"q{i}" for i in range(n)]
    for q in qubits:
        pushQubit(q, [1, 1])
    
    optimalTurns = int(np.pi / 4 * np.sqrt(2**n) - 0.5)
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

# Run test
groverSearch(6)

