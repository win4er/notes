import numpy as np
from scipy.integrate import solve_ivp
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.patches import Rectangle, Circle
from matplotlib.transforms import Affine2D

# На будущее: следовать стандарту PEP8 и не забывать про условный if __name__ == '__main__'

# Параметры системы
m1, m2, l, g = 1.0, 1, 0.5, 9.81
alpha = np.pi / 4

# Начальные условия
s0, phi0, s_dot0, phi_dot0 = 0.0, np.pi, 0.0, 0
t_span = [0, 10]

# Уравнения движения:
# Не забываем про уравнения лагранжа 2 рода:
# (m1+m2)(s''-g*sin(alpha))-m2*l*[phi''*cos(phi-alpha)-phi'^2*sin(phi-alpha)] = 0
def sys_eq(t, y):
    s, phi, sd, phid = y
    A = [[m1+m2, -m2*l*np.cos(phi-alpha)], [-np.cos(phi-alpha), l]]
    B = [(m1+m2)*g*np.sin(alpha)-m2*l*phid**2*np.sin(phi-alpha), -g*np.sin(phi)]
    sdd, phidd = np.linalg.solve(A, B)
    return [sd, phid, sdd, phidd]

# Решение
sol = solve_ivp(sys_eq, t_span, [s0, phi0, s_dot0, phi_dot0], dense_output=True, max_step=0.01)
t = np.linspace(t_span[0], t_span[1], 501)
s, phi, sd, phid = sol.sol(t)

# Усилие N
def calc_N(i):
    A = [[m1+m2, -m2*l*np.cos(phi[i]-alpha)], [-np.cos(phi[i]-alpha), l]]
    B = [(m1+m2)*g*np.sin(alpha)-m2*l*phid[i]**2*np.sin(phi[i]-alpha), -g*np.sin(phi[i])]
    sdd, _ = np.linalg.solve(A, B)
    return m2*(g*np.cos(phi[i]) + l*phid[i]**2 + sdd*np.sin(phi[i]-alpha))

N = np.array([calc_N(i) for i in range(len(t))])

# Графики
fig, axs = plt.subplots(2, 2, figsize=(12, 8))
axs[0,0].plot(t, s, 'b-', linewidth=2); axs[0,0].set_title('s(t)'); axs[0,0].grid(True, alpha=0.3)
axs[0,1].plot(t, phi, 'r-', linewidth=2); axs[0,1].set_title('φ(t)'); axs[0,1].grid(True, alpha=0.3)
axs[1,0].plot(t, N, 'g-', linewidth=2); axs[1,0].set_title('N(t)'); axs[1,0].grid(True, alpha=0.3)
axs[1,0].axhline(0, color='k', linestyle='--', alpha=0.3)

# Анимация
ax = axs[1,1]
ax.set(xlim=(-6,6), ylim=(-4,4), aspect='equal', title='Анимация системы')
ax.grid(True, alpha=0.3)

# Плоскость
L = 8
ax.plot(
    [-L*np.cos(alpha), L*np.cos(alpha)],
    [-L*np.sin(alpha), L*np.sin(alpha)],
    'k-',
    linewidth=3
)

# Элементы
rect = Rectangle((-0.15, -0.1), 0.3, 0.2, fc='blue', alpha=0.7, ec='black')
point = Circle((0,0), 0.05, fc='red', ec='black')
rod, = ax.plot([], [], 'r-', linewidth=2)
traj, = ax.plot([], [], 'g--', linewidth=1, alpha=0.5)
ax.add_patch(rect); ax.add_patch(point)

def init():
    cx, cy = s[0]*np.cos(alpha), s[0]*np.sin(alpha)
    rect.set_xy((-0.15, -0.1))
    rect.set_transform(Affine2D().rotate_deg_around(cx, cy, alpha*180/np.pi) + ax.transData)
    rect.set_xy((cx-0.15, cy-0.1))
    Bx, By = cx + l*np.sin(phi[0]), cy - l*np.cos(phi[0])
    point.center = (Bx, By)
    rod.set_data([cx, Bx], [cy, By])
    traj.set_data([], [])
    update.tx, update.ty = [], []
    return rect, rod, point, traj

# Тот самый kadr из методички
def update(frame):
    idx = min(frame, len(t)-1)
    cx, cy = -s[idx]*np.cos(alpha), -s[idx]*np.sin(alpha)+0.1
    rect.set_xy((cx-0.15, cy-0.1))
    rect.set_transform(Affine2D().rotate_deg_around(cx, cy, alpha*180/np.pi) + ax.transData)
    Bx, By = cx + l*np.sin(phi[idx]), cy - l*np.cos(phi[idx])
    point.center = (Bx, By)
    rod.set_data([cx, Bx], [cy, By])
    update.tx.append(Bx); update.ty.append(By)
    if len(update.tx) > 100: update.tx.pop(0); update.ty.pop(0)
    traj.set_data(update.tx, update.ty)
    return rect, rod, point, traj

ani = animation.FuncAnimation(
    fig,
    update,
    frames=range(0, len(t), 2),
    init_func=init, blit=True,
    interval=20,
    repeat=True
)
plt.tight_layout()
plt.show()