#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// PCL 헤더
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

// 좌표를 저장할 벡터
std::vector<pcl::PointXYZ> selected_points;

// 마우스 이벤트 콜백 함수
void areaPickingCallback(const pcl::visualization::AreaPickingEvent& event, void* viewer_void) {
  std::vector<int> indices;
  if (event.getPointsIndices(indices) == -1) {
    return;
  }

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = *(pcl::PointCloud<pcl::PointXYZ>::Ptr*)viewer_void;
  for (const auto& index : indices) {
    selected_points.push_back(cloud->points[index]);
  }

  std::cout << "Points selected: " << indices.size() << std::endl;
  for (const auto& point : selected_points) {
    std::cout << point << std::endl;
  }

  // 선택된 점들을 시각화 (선 연결)
  pcl::visualization::PCLVisualizer* viewer = static_cast<pcl::visualization::PCLVisualizer*>(viewer_void);
  for (size_t i = 1; i < selected_points.size(); ++i) {
      viewer->addLine(selected_points[i-1], selected_points[i], 0.0, 1.0, 0.0, "line_" + std::to_string(i));
  }
}

// .csv 파일로 저장
void saveToCSV(const std::vector<pcl::PointXYZ>& points, const std::string& filename) {
  std::ofstream file;
  file.open(filename);

  if (file.is_open()) {
    for (const auto& point : points) {
        file << point.x << "," << point.y << "," << point.z << "\n";
    }
    file.close();
    std::cout << "Points saved to " << filename << std::endl;
  } else {
    std::cerr << "Could not open the file!" << std::endl;
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input.pcd>" << std::endl;
    return -1;
  }

  // PCL 객체 생성 및 .pcd 파일 로드
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
  if (pcl::io::loadPCDFile<pcl::PointXYZ>(argv[1], *cloud) == -1) {
    PCL_ERROR("Couldn't read file\n");
    return -1;
  }

  // PCLVisualizer 객체 생성
  pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
  viewer->setBackgroundColor(0, 0, 0);
  viewer->addPointCloud<pcl::PointXYZ>(cloud, "sample cloud");
  viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");
  viewer->addCoordinateSystem(1.0);
  viewer->initCameraParameters();

  // 마우스 이벤트 콜백 설정
  viewer->registerAreaPickingCallback(areaPickingCallback, (void*)viewer.get());

  // 뷰어 루프
  while (!viewer->wasStopped()) {
    viewer->spinOnce(100);
  }

  // 선택된 좌표를 .csv 파일로 저장
  saveToCSV(selected_points, "selected_points.csv");

  return 0;
}
