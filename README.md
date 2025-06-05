# OpenGL Bunny Rendering Meshes - README

## 📝 Assignment 8 Overview

This project renders a high-resolution bunny mesh using OpenGL in two different modes and reports performance as frames-per-second (FPS).

### ✅ Assignment Requirements

#### 🐰 Triangle Mesh

* The bunny mesh is stored in `bunny.obj`.
* It contains **69,451 triangles**.
* `load_mesh.cpp` handles loading mesh data into:

  * `gPositions`: vertex positions
  * `gNormals`: vertex normals
  * `gTriangles`: triangle index list

#### 🔁 Transformation Parameters

* Scale bunny by **10×**.
* Translate to **(0.1, −1.0, −1.5)**.
* Camera:

  * Position the camera at e = (0, 0, 0)
  * coordinate axes u = (1, 0, 0), v =(0, 1, 0), and w = (0, 0, 1)
* Perspective projection:

  * l = -0.1, r = 0.1
  * b = -0.1, t = 0.1
  * n = 0.1, f = 1000.0
* Viewport: 1280 × 1280

#### 💡 Shading Parameters

* Enable depth buffer
* Disable back-face culling
* Material:

  * Ambient (ka) = Diffuse (kd) = (1, 1, 1)
  * Specular (ks) = (0, 0, 0), shininess p = 0
* Ambient light: **Ia = (0.2, 0.2, 0.2)**
* Directional light:

  * Direction: **(−1, −1, −1)** (normalized)
  * Ambient = (0, 0, 0)
  * Diffuse = (1, 1, 1)
  * Specular = (0, 0, 0)

#### ⏱️ FPS Reporting

* FPS calculated using `frame_timer.cpp`
* Displayed in GLUT window title every frame

---

## ❓ Questions

### Q1: Immediate Mode

* Render using `glBegin(GL_TRIANGLES)` and per-vertex `glVertex3f`, `glNormal3f`

### Q2: Vertex Arrays

* Use VAO + VBO:

  * Store vertex positions, normals, and indices
  * Copy all the data to GPU memory
  * Render with `glDrawElements`


## ⚙️ How to Compile and Run

### ✅ Requirements

* Visual Studio Community 2022
* All required DLLs (GLEW, GLUT) are included in the project folder

### 🛠️ Clone & Build Instructions (Using Git + Visual Studio)
##📥 1. Clone the Repository

```
git clone https://github.com/tuguldur77/CG_HW8_202213827.git
cd OpenGL_Bunny_Renderer
```

1. Open folder and click `OpenglViewer.sln` in Visual Studio 2022
2. Build the solution (Ctrl + Shift + B)
3. Run with Debug (F5)

---

## 🎮 Controls

* Press `1` on keyboard: Immediate Mode
* Press `2` on keyboard: Vertex Arrays Mode
* Press `ESC` on keyboard: Exit the application

---

### 🐰 Outputs:
Q1: Immediate Mode output:
![Q1_output](https://github.com/user-attachments/assets/e4d92dfd-8764-4855-b9a4-31b858ef63a4)

Q2: Vertex Arrays output:
![Q2_output](https://github.com/user-attachments/assets/9393b801-648a-4a1e-b5cf-9fe684ce6d7b)

Terminal output:
![Output_terminal](https://github.com/user-attachments/assets/e089fc8b-9516-4c83-a654-ce190bfdd250)

## 👨‍💻 Author
Tuguldur SID: 202213827 / Konkuk University — Department of Computer Science & Engineering / Computer Graphics (Spring 2025



