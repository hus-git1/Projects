import tkinter as tk
from collections import defaultdict, deque
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.animation as animation
import itertools
##CORRECT WITH LEGENF BEING SHOWN IN NFA ONYL
# Define the NFA class
class NFA:
    def __init__(self, states, alphabet, transitions, start_state, final_states):
        self.states = states
        self.alphabet = alphabet
        self.transitions = transitions
        self.start_state = start_state
        self.final_states = final_states

    def process_string(self, string):
        current_states = {self.start_state}
        states_sequence = [current_states.copy()]
        transitions_sequence = []
        accepted = False

        for symbol in string:
            transitions_sequence.append(f"{current_states} --{symbol}--> ")
            new_states = set()
            for state in current_states:
                if (state, symbol) in self.transitions:
                    new_states.update(self.transitions[(state, symbol)])
            current_states = new_states
            states_sequence.append(current_states.copy())
            transitions_sequence[-1] += f"{current_states}"

        if current_states & self.final_states:
            accepted = True

        return states_sequence, transitions_sequence, accepted

    def to_dfa(self):
        dfa_states = set()
        dfa_start_state = frozenset([self.start_state])
        dfa_final_states = set()
        dfa_transitions = dict()

        queue = deque([dfa_start_state])
        visited = set()

        while queue:
            current = queue.popleft()
            if current in visited:
                continue
            visited.add(current)
            dfa_states.add(current)

            if current & self.final_states:
                dfa_final_states.add(current)

            for symbol in self.alphabet:
                next_state = frozenset(
                    state
                    for nfa_state in current
                    if (nfa_state, symbol) in self.transitions
                    for state in self.transitions[(nfa_state, symbol)]
                )
                dfa_transitions[(current, symbol)] = next_state
                if next_state not in visited and next_state:
                    queue.append(next_state)

        return DFA(dfa_states, self.alphabet, dfa_transitions, dfa_start_state, dfa_final_states)

class DFA:
    def __init__(self, states, alphabet, transitions, start_state, final_states):
        self.states = states
        self.alphabet = alphabet
        self.transitions = transitions
        self.start_state = start_state
        self.final_states = final_states

    def process_string(self, string):
        current_state = self.start_state
        states_sequence = [current_state]
        transitions_sequence = []
        accepted = False

        for symbol in string:
            transitions_sequence.append(f"{current_state} --{symbol}--> ")
            if (current_state, symbol) in self.transitions:
                current_state = self.transitions[(current_state, symbol)]
            else:
                current_state = None
            states_sequence.append(current_state)
            transitions_sequence[-1] += f"{current_state}"

        if current_state in self.final_states:
            accepted = True

        return states_sequence, transitions_sequence, accepted

    def minimize(self):
    # Step 1: Initialize partition P
        final_states = frozenset(self.final_states)
        non_final_states = frozenset(self.states - self.final_states)
    
        P = {final_states, non_final_states} if non_final_states else {final_states}
        W = {final_states}

        while W:
            A = W.pop()
            for symbol in self.alphabet:
                # X is the set of states for which the transition on 'symbol' leads to a state in A
                X = frozenset({state for state in self.states if self.transitions.get((state, symbol)) in A})
                new_P = set()
                for Y in P:
                    intersection = X & Y
                    difference = Y - X
                    if intersection and difference:
                        new_P.add(intersection)
                        new_P.add(difference)
                        if Y in W:
                            W.remove(Y)
                            W.add(intersection)
                            W.add(difference)
                        else:
                            if len(intersection) <= len(difference):
                                W.add(intersection)
                            else:
                                W.add(difference)
                    else:
                        new_P.add(Y)
                    P = new_P

        new_states = {frozenset(group) for group in P}
        new_start_state = next(frozenset(group) for group in P if self.start_state in group)
        new_final_states = {frozenset(group) for group in P if group & self.final_states}

        new_transitions = {}
        for group in P:
            representative = next(iter(group))
            for symbol in self.alphabet:
                target = self.transitions.get((representative, symbol))
                if target:
                    target_group = next(frozenset(g) for g in P if target in g)
                    new_transitions[(frozenset(group), symbol)] = target_group

        return DFA(new_states, self.alphabet, new_transitions, new_start_state, new_final_states)

class NFA_GUI:
    def __init__(self, master):
        self.master = master
        self.master.title("NFA to DFA Simulator with Real-time Transition")

        self.create_widgets()
        self.nfa = None
        self.dfa = None
        self.min_dfa = None
        self.fig = Figure()
        self.ax = self.fig.add_subplot(111)
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
        self.canvas.get_tk_widget().grid(row=12, columnspan=2)

    def create_widgets(self):
        # GUI components setup
        tk.Label(self.master, text="States (comma-separated):").grid(row=0, column=0)
        self.states_entry = tk.Entry(self.master)
        self.states_entry.grid(row=0, column=1)

        tk.Label(self.master, text="Alphabet (comma-separated):").grid(row=1, column=0)
        self.alphabet_entry = tk.Entry(self.master)
        self.alphabet_entry.grid(row=1, column=1)

        tk.Label(self.master, text="Transitions (format: state,symbol->state;):").grid(row=2, column=0)
        self.transitions_entry = tk.Entry(self.master)
        self.transitions_entry.grid(row=2, column=1)

        tk.Label(self.master, text="Start State:").grid(row=3, column=0)
        self.start_state_entry = tk.Entry(self.master)
        self.start_state_entry.grid(row=3, column=1)

        tk.Label(self.master, text="Final States (comma-separated):").grid(row=4, column=0)
        self.final_states_entry = tk.Entry(self.master)
        self.final_states_entry.grid(row=4, column=1)

        tk.Label(self.master, text="Test String:").grid(row=5, column=0)
        self.test_string_entry = tk.Entry(self.master)
        self.test_string_entry.grid(row=5, column=1)

        tk.Button(self.master, text="Simulate and Animate NFA", command=self.run_nfa).grid(row=6, column=0)
        tk.Button(self.master, text="Convert to DFA", command=self.convert_to_dfa).grid(row=6, column=1)
        self.result_label = tk.Label(self.master, text="")
        self.result_label.grid(row=7, columnspan=2)

        self.transitions_label = tk.Text(self.master, height=4, width=50)
        self.transitions_label.grid(row=9, columnspan=2)

        tk.Button(self.master, text="Simulate and Animate DFA", command=self.run_dfa).grid(row=8, columnspan=2)
        tk.Button(self.master, text="Minimize DFA", command=self.minimize_dfa).grid(row=10, columnspan=2)
        tk.Button(self.master, text="Simulate and Animate Minimized DFA", command=self.run_min_dfa).grid(row=11, columnspan=2)

    def parse_transitions(self):
        transitions = defaultdict(set)
        for part in self.transitions_entry.get().split(';'):
            if '->' in part:
                left, right = part.split('->')
                state, symbol = left.split(',')
                transitions[(state.strip(), symbol.strip())].add(right.strip())
        return dict(transitions)

    def run_nfa(self):
        states = set(self.states_entry.get().split(','))
        alphabet = set(self.alphabet_entry.get().split(','))
        transitions = self.parse_transitions()
        start_state = self.start_state_entry.get()
        final_states = set(self.final_states_entry.get().split(','))
        test_string = self.test_string_entry.get()

        self.nfa = NFA(states, alphabet, transitions, start_state, final_states)
        sequence, transitions_sequence, accepted = self.nfa.process_string(test_string)
        self.result_label.config(text="String accepted by NFA" if accepted else "String not accepted by NFA")
        self.transitions_label.delete('1.0', tk.END)
        self.transitions_label.insert('1.0', "\n".join(transitions_sequence))
        self.animate_nfa(sequence)

    def convert_to_dfa(self):
        if self.nfa:
            self.dfa = self.nfa.to_dfa()
            tk.messagebox.showinfo("Conversion Successful", "NFA has been successfully converted to DFA")

    def run_dfa(self):
        if not self.dfa:
            tk.messagebox.showerror("Error", "Please convert NFA to DFA first")
            return

        test_string = self.test_string_entry.get()
        sequence, transitions_sequence, accepted = self.dfa.process_string(test_string)
        self.result_label.config(text="String accepted by DFA" if accepted else "String not accepted by DFA")
        self.transitions_label.delete('1.0', tk.END)
        self.transitions_label.insert('1.0', "\n".join(transitions_sequence))
        self.animate_dfa(sequence)

    def minimize_dfa(self):
        if self.dfa:
            self.min_dfa = self.dfa.minimize()
            tk.messagebox.showinfo("Minimization Successful", "DFA has been successfully minimized")

    def run_min_dfa(self):
        if not self.min_dfa:
            tk.messagebox.showerror("Error", "Please minimize the DFA first")
            return

        test_string = self.test_string_entry.get()
        sequence, transitions_sequence, accepted = self.min_dfa.process_string(test_string)
        self.result_label.config(text="String accepted by Minimized DFA" if accepted else "String not accepted by Minimized DFA")
        self.transitions_label.delete('1.0', tk.END)
        self.transitions_label.insert('1.0', "\n".join(transitions_sequence))
        self.animate_dfa(sequence, minimized=True)

    def animate_nfa(self, sequence):
        G = nx.DiGraph()
        for (state, symbol), next_states in self.nfa.transitions.items():
            for next_state in next_states:
                G.add_edge(state, next_state, label=symbol)

        pos = nx.circular_layout(G)

        def update(num):
            self.ax.clear()
            self.ax.set_title(f"NFA State Transition: Step {num}")
            nx.draw(G, pos, with_labels=True, ax=self.ax, node_color='skyblue', edge_color='k', style='dotted')
            edge_labels = nx.get_edge_attributes(G, 'label')
            nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, ax=self.ax)
            current_states = list(sequence[num])
            nx.draw_networkx_nodes(G, pos, nodelist=current_states, node_color='red', node_size=500, ax=self.ax, label='Current State')
            nx.draw_networkx_nodes(G, pos, nodelist=[self.nfa.start_state], node_color='green', node_size=500, ax=self.ax, label='Start State')
            nx.draw_networkx_nodes(G, pos, nodelist=list(self.nfa.final_states), node_color='blue', node_shape='o', node_size=500, ax=self.ax, label='Final State')
            if num > 0:
                previous_states = list(sequence[num-1])
                edges = nx.draw_networkx_edges(G, pos, edgelist=[(u, v) for u in previous_states for v in current_states], ax=self.ax, edge_color='red', style='dashed')
            
            red_patch = plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='red', markersize=10, label='Current State')
            green_patch = plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='green', markersize=10, label='Start State')
            blue_patch = plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='blue', markersize=10, label='Final State')
            self.ax.legend(handles=[red_patch, green_patch, blue_patch], loc='best')
            self.canvas.draw()
        ani = animation.FuncAnimation(self.fig, update, frames=len(sequence), interval=1000, repeat=False)
        self.canvas.draw()

    def animate_dfa(self, sequence, minimized=False):
        dfa = self.min_dfa if minimized else self.dfa
        G = nx.DiGraph()
        for (state, symbol), next_state in dfa.transitions.items():
            G.add_edge(state, next_state, label=symbol)

        pos = nx.circular_layout(G)

        def update(num):
            self.ax.clear()
            title = f"Minimized DFA State Transition: Step {num}" if minimized else f"DFA State Transition: Step {num}"
            self.ax.set_title(title)
            nx.draw(G, pos, with_labels=True, ax=self.ax, node_color='skyblue', edge_color='k', style='dotted')
            edge_labels = nx.get_edge_attributes(G, 'label')
            nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, ax=self.ax)
            current_state = sequence[num]
            nx.draw_networkx_nodes(G, pos, nodelist=[current_state], node_color='red', node_size=500, ax=self.ax, label='Current State')
            nx.draw_networkx_nodes(G, pos, nodelist=[dfa.start_state], node_color='green', node_size=500, ax=self.ax, label='Start State')
            nx.draw_networkx_nodes(G, pos, nodelist=list(dfa.final_states), node_color='blue', node_shape='o', node_size=500, ax=self.ax, label='Final State')
            if num > 0:
                previous_state = sequence[num-1]
                edges = nx.draw_networkx_edges(G, pos, edgelist=[(previous_state, current_state)], ax=self.ax, edge_color='red', style='dashed')
            self.canvas.draw()

        ani = animation.FuncAnimation(self.fig, update, frames=len(sequence), interval=1000, repeat=False)
        self.canvas.draw()

    def show_automaton(self, automaton):
        G = nx.DiGraph()
        for (state, symbol), next_state in automaton.transitions.items():
            G.add_edge(state, next_state, label=symbol)

        pos = nx.circular_layout(G)

        self.ax.clear()
        self.ax.set_title("Automaton")
        nx.draw(G, pos, with_labels=True, ax=self.ax, node_color='skyblue', edge_color='k', style='dotted')
        edge_labels = nx.get_edge_attributes(G, 'label')
        nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, ax=self.ax)
        nx.draw_networkx_nodes(G, pos, nodelist=[automaton.start_state], node_color='green', node_size=500, ax=self.ax, label='Start State')
        nx.draw_networkx_nodes(G, pos, nodelist=list(automaton.final_states), node_color='blue', node_shape='o', node_size=500, ax=self.ax, label='Final State')
        nx.draw_networkx_edges(G, pos, ax=self.ax, connectionstyle='arc3,rad=0.2')
        red_patch = plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='red', markersize=10, label='Current State')
        green_patch = plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='green', markersize=10, label='Start State')
        blue_patch = plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='blue', markersize=10, label='Final State')
        self.ax.legend(handles=[red_patch, green_patch, blue_patch], loc='best')
        self.canvas.draw()

if __name__ == "__main__":
    root = tk.Tk()
    gui = NFA_GUI(root)
    root.mainloop()
