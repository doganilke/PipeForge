# PipeForge: DBSCAN Image Clustering Pipeline

PipeForge is a modular C++ pipeline framework for processing images using a DBSCAN (Density-Based Spatial Clustering of Applications with Noise) clustering algorithm. It supports image input via local files or HTTP(S) URLs and uses OpenCV for image manipulation and contour detection.

## ✨ Features

- 🌐 Download images via URL using `libcurl`
- 🧹 Preprocess images to extract contours and center points
- 📊 Apply DBSCAN clustering to identify groups of points
- 🎯 Visualize results with cluster-colored overlay and contour image
- 🔌 Modular pipeline design allows flexible processing steps

---

## 📂 Project Structure

.
├── include/ # Header files
├── src/ # Source files
├── CMakeLists.txt # CMake build script
├── Dockerfile # Optional container setup
└── README.md # This file


---

## 🔧 Dependencies

- **C++17**
- [OpenCV](https://opencv.org/)
- [Boost.ProgramOptions](https://www.boost.org/doc/libs/)
- [libcurl](https://curl.se/libcurl/)
- [nanoflann](https://github.com/jlblancoc/nanoflann) (fetched automatically)

---

## ⚙️ Build Instructions

### 🔨 Build (with CMake)

```bash
git clone https://github.com/your_username/PipeForge.git
cd PipeForge
mkdir build && cd build
cmake ..
cmake --build .


🐳 Optional: Build with Docker

docker build -t pipeforge .
docker run --rm -v $(pwd):/data pipeforge ./ClusteringRows --path /data/your_image.jpg

🚀 Usage
./ClusteringRows --path /path/to/image.jpg
./ClusteringRows --path https://example.com/image.jpg

Output

centerImage.jpg: Clusters drawn as colored circles
contours.jpg: Original image with contours

📷 Example

📐 DBSCAN Parameters
You can configure DBSCAN via:

const int MINIMUM_POINTS = 10;  // Minimum points to form a cluster
const float EPSILON = 150.0f;   // Radius for neighborhood search


Modify these in dbscanpipe.cpp to tune clustering.

🧱 Pipeline Architecture

Each processing step implements the IPipeline interface and links to the next via .next():
downloader->next(preprocess.get())
          ->next(dbscan.get())
          ->next(output.get());

🧑‍💻 Author
İlke Doğan

GitHub: doganilke

📝 License
This project is licensed under the MIT License. See LICENSE for details.