import cv2
import numpy as np
import os
import sys

# P.S. В комментариях я называю 1 и 4 точки главными, а 2 и 3 дополнительными
# а ещё пишу кривая, хотя это сплайн :)

# ================== Управление ==================
# ЛКМ - добавить точку кривой
# ПКМ - удалить точку кривой (если это главная точка и не первая)
# Колесо мыши - смещение картинки (кликом, без перетаскивания)
# ЛКМ по точке - перетаскивание точки
# V - переключение между режимами редактирования и просмотра (ЕСЛИ НЕ РИСУЕТСЯ - ты в режиме просмотра)
# S - сохранение точек в файлы (all_points.txt и control_points.txt) (только в режиме просмотра)
# R - чтение точек из файла control_points.txt (только в режиме просмотра)
# ESC - выход
# ===============================================

# ================== HOW TO USE ==================
# 1. Передал путь до картинки уровня параметром
# 2. Понадабавлял точек, подвигал их как надо
# 3. Чекнул как оно выглядит через 'v'
# 4. Если всё нравится, нажимаешь 's' для сохранения в файлы
# 5. Потом можно нажать 'r' для загрузки контрольных точек из файла
# 6. Нажимаешь ещё раз 'v' и снова добавляешь точки
# 7. Повторять пока не нарисуешь все кривые
# ===============================================

# Глобальные переменные
control_points = []  # Все точки кривой (для рисования берутся по 4)
all_points = []
dragging = None      # Индекс точки, которую сейчас перетаскиваем
finished = False     # Флаг для отображения законченной кривой
offset = [0, 0]      # Смещение картинки

WINDOW_SIZE = (1280, 720)  # Размер окна

if len(sys.argv) < 2:
    print(f"Usage: python {sys.argv[0]} <path_to_image>")
    sys.exit(1)

picture = cv2.imread(sys.argv[1], cv2.IMREAD_UNCHANGED)
# Фикс для картинок с альфа-каналом
picture[picture[:, :, 3] < 30] = [255, 255, 255, 255]

# Создание пустой картинки
img = np.zeros((WINDOW_SIZE[1], WINDOW_SIZE[0], 3), np.uint8)
windowName = 'Cubic Bezier Spline'
cv2.namedWindow(windowName)

def is_near_control_point(x1, y1):
    for i, (x2, y2) in enumerate(control_points):
        if abs(x1 - x2) < 5 and abs(y1 - y2) < 5:
            return i
    return None

# Отрисовка кривой Безье по 4 точкам с динамическим числом точек,
# зависящим от расстояния между главными точками (p0 и p3)
def draw_bezier_curve(control_points):
    pts = []
    p0, p1, p2, p3 = control_points
    distance = np.sqrt((p3[0] - p0[0])**2 + (p3[1] - p0[1])**2)
    num_points = int(distance / 10)
    
    for t in np.linspace(0, 1, num_points):
        x = int((1-t)**3 * p0[0] + 3*(1-t)**2 * t * p1[0] + 3*(1-t) * t**2 * p2[0] + t**3 * p3[0])
        y = int((1-t)**3 * p0[1] + 3*(1-t)**2 * t * p1[1] + 3*(1-t) * t**2 * p2[1] + t**3 * p3[1])
        pts.append((x, y))
        cv2.circle(img, (x, y), 1, (255, 0, 0), -1)
    return pts

def add_point(x, y):
    global control_points
    if len(control_points) == 0:
        # Первая точка спавнит лишь 2 точки (себя и 1 дополнительную)
        control_points.append((x, y))
        control_points.append((x + 30, y))
        return
    # В обычном случае добавляем 3 точки
    # (дополнительные точки направлены в сторону предыдущей главной точки)
    delta = (x - control_points[-2][0], y - control_points[-2][1])
    control_points.append((x - delta[0] // 3, y - delta[1] // 3))
    control_points.append((x, y))
    control_points.append((x + delta[0] // 3, y + delta[1] // 3))

def switch_finish():
    global control_points, finished
    if len(control_points) < 2:
        finished = not finished
        return
    if not finished:
        extra_sup_point_delta = (control_points[1][0] - control_points[0][0],
                                 control_points[1][1] - control_points[0][1])
        control_points.append((control_points[0][0] - extra_sup_point_delta[0],
                               control_points[0][1] - extra_sup_point_delta[1]))
        control_points.append((control_points[0][0], control_points[0][1]))
    else:
        control_points = control_points[:-2]
    finished = not finished

def drag_point(i, x, y):
    if i == 0 or i == 1:
        control_points[i] = (x, y)
        return
    if i % 3 == 0:
        prev_delta = (control_points[i][0] - control_points[i-1][0],
                      control_points[i][1] - control_points[i-1][1])
        next_delta = (control_points[i+1][0] - control_points[i][0],
                      control_points[i+1][1] - control_points[i][1])
        control_points[i-1] = (x - prev_delta[0], y - prev_delta[1])
        control_points[i] = (x, y)
        control_points[i+1] = (x + next_delta[0], y + next_delta[1])
    elif i % 3 == 1:
        delta = (x - control_points[i-1][0], y - control_points[i-1][1])
        control_points[i] = (x, y)
        control_points[i-2] = (x - 2 * delta[0], y - 2 * delta[1])
    elif i % 3 == 2:
        delta = (x - control_points[i+1][0], y - control_points[i+1][1])
        control_points[i] = (x, y)
        control_points[i+2] = (x - 2 * delta[0], y - 2 * delta[1])

def mouse_callback(event, x, y, flags, param):
    global control_points, dragging, finished, offset
    # Корректируем координаты с учетом смещения
    x, y = x + offset[0], y + offset[1]
    if event == cv2.EVENT_LBUTTONDOWN and not finished:
        dragging = is_near_control_point(x, y)
        if dragging is None:
            add_point(x, y)
    elif event == cv2.EVENT_LBUTTONUP:
        dragging = None
    elif event == cv2.EVENT_RBUTTONDOWN and not finished:
        point = is_near_control_point(x, y)
        if point is not None and point % 3 == 0 and point != 0:
            control_points[point-1:point+2] = []
    elif event == cv2.EVENT_MBUTTONDOWN:
        change_offset(x, y)
    elif event == cv2.EVENT_MOUSEMOVE and dragging is not None:
        drag_point(dragging, x, y)

def draw_control_points():
    global all_points, control_points
    if not finished:
        for i, point in enumerate(control_points):
            disp_point = (point[0] - offset[0], point[1] - offset[1])
            if i % 3 == 0:
                cv2.circle(img, disp_point, 3, (0, 0, 255), -1)
            else:
                cv2.circle(img, disp_point, 3, (255, 0, 255), -1)
    all_points = []
    for i in range(3, len(control_points), 3):
        points = [(x - offset[0], y - offset[1]) for x, y in control_points[i-3:i+1]]
        all_points += draw_bezier_curve(points)

def draw_picture():
    global picture, img, offset
    img = picture[offset[1]:offset[1]+WINDOW_SIZE[1],
                  offset[0]:offset[0]+WINDOW_SIZE[0]].copy()
    draw_control_points()
    cv2.imshow(windowName, img)

def change_offset(x, y):
    global offset
    offset = [x - WINDOW_SIZE[0] // 2, y - WINDOW_SIZE[1] // 2]
    offset[0] = max(0, min(offset[0], picture.shape[1] - WINDOW_SIZE[0]))
    offset[1] = max(0, min(offset[1], picture.shape[0] - WINDOW_SIZE[1]))

cv2.setMouseCallback(windowName, mouse_callback)

while True:
    draw_picture()
    key = cv2.waitKey(20) & 0xFF

    if key == ord('v'):
        switch_finish()

    if key == ord('s') and finished:
        print("Saving points to files: all_points.txt and control_points.txt")
        # Save all_points in C++ 2D-list format: {{x1, y1}, {x2, y2}, ... }
        with open('all_points.txt', 'w') as f:
            json_list_str = '[ ' + ', '.join(f'[ {x}, {y} ]' for (x, y) in all_points) + ' ]'
            f.write(json_list_str + '\n')
        # Save control_points using Python list format
        with open('control_points.txt', 'w') as f:
            f.write(str(control_points) + '\n')

    if key == ord('r') and finished:
        print("Reading control points from control_points.txt")
        if not os.path.exists('control_points.txt'):
            with open('control_points.txt', 'w') as f:
                f.write("[]\n")
            control_points = []
        else:
            with open('control_points.txt', 'r') as f:
                line = f.readline()
                line = line if line else "[]"
                control_points = eval(line)
    
    if key == 27:  # Exit on 'ESC' key
        break

cv2.destroyAllWindows()
