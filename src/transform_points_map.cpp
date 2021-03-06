#include "ros/ros.h"
#include "tf/tf.h"
#include "pcl_ros/transforms.h"
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "transform_points_map");
    ros::NodeHandle nh("~");
    std::string map_in_path;
    std::string map_out_path;

    tf::TransformListener local_transform_listener;
    tf::StampedTransform local_transform;

    try
    {
        ros::Time now = ros::Time(0);
        local_transform_listener.waitForTransform("/map1", "/map", now, ros::Duration(10.0));
        local_transform_listener.lookupTransform("/map1", "/map", now, local_transform);
    }
    catch (tf::TransformException &ex)
    {
        ROS_ERROR("%s", ex.what());
    }

    nh.param("map_in_path", map_in_path, std::string("/home/zmy/data1/bag/forklift/xibu_map/binary_pcd/binary_pcd_002.pcd"));
    nh.param("map_out_path", map_out_path, map_in_path);
    std::cout << "map_in_path" << map_in_path << std::endl;
    std::cout << "map_out_path" << map_out_path << std::endl;
    if (map_in_path == "")
    {
        ROS_ERROR("map_in_path is null");
        return -1;
    }
    pcl::PointCloud<pcl::PointXYZI> cloud_in;
    pcl::PointCloud<pcl::PointXYZI> cloud_out;
    pcl::io::loadPCDFile(map_in_path.c_str(), cloud_in);
    std::cout << "has load" << map_in_path << std::endl;
    pcl_ros::transformPointCloud(cloud_in, cloud_out, local_transform);
    std::cout << "has transformed map" << std::endl;
    // if (access(down_sample_path.c_str(), 0) != 0)
    // {
    //     mkdir(down_sample_path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
    // }/home/zmy/data1/bag/forklift/xibu_map/binary_pcd/binary_pcd_002.pcd
    pcl::io::savePCDFileBinary(map_out_path, cloud_out);
    std::cout << "turn over " << std::endl;
    return 0;
}