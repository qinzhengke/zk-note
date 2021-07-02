VTK：3D可视化库{#cpp_lib_vtk}
===================

\section vtk_useful_example 一个有用的第三方Example网站

https://kitware.github.io/vtk-examples/site/

这个网站上有非常多的示例代码，非常推荐，学习vtk一定要过一遍。


\section vtk_useless_example 无用的Example

VTK的Example没什么用处，甚至存在误导，不要参考。

VTK源码下存在Example目录，但是即使给cmake加上“-DVTK_BUILD_EXAMPLES=ON”参数，仍然没有编译出example。
一探究竟，发现库的作者已经不怎么维护Example了：
(1)Example使用add_test方式编译，但不知道最后的可执行文件在哪。
(2)Example的CMakeLists.txt存在大量注释掉的Example
(3)Example的相关cmake配置存在不用的接口，主要是8.x版本的接口，已经和9.x版本不适配。例如：只有调用但是没有定义的vtk_module_config（当时还找了半天定义）。


\section vtk_trouble_shooting Trouble Shooting

\subsection vtk_x11_not_found X11_Xt_LIB could not be found.

完整的问题：
```
CMake Error at Rendering/OpenGL/CMakeLists.txt:304 (message):
   X11_Xt_LIB could not be found.  Required for VTK X lib.
```

解决方法：

```
sudo apt install libxt-dev
```

\subsection vtk_no_member_TupleValue

 完整问题：　
 ```
 error: ‘class vtkUnsignedCharArray’ has no member named ‘InsertNextTupleValue’; did you mean ‘InsertNextTypedTuple’?
```

TupleValue在>7.2版本后改名为TypedTuple，https://kitware.github.io/vtk-examples/site/Cxx上面的example，明明是8.2能编过，但是还是用了这个接口，是因为它的代码中做了宏定义，转到了最新的命名。

\subsection vtk_incomplete_type

完整问题：
```
error: invalid use of incomplete type ‘class vtkPointData’
```

原因是没有加入include<vtkPointData.h>，实际上vtkPointData只有class声明，没有具体的定义，拿指针没问题，但是对指针进行解引用，就会出错。

### 使用案例

cmake_minimum_required(VERSION 3.12)

# 使用文件夹名称作为project名称
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
message("[${PROJECT_NAME}] cmake in Debug mode")
add_compile_options(-O0 -g -ggdb)
else()
message("[${PROJECT_NAME}] in Release mode")
endif()

find_package(VTK 8.2 REQUIRED COMPONENTS
    vtkCommonColor
    vtkCommonCore
    vtkFiltersGeneral
    vtkFiltersCore
    vtkFiltersPoints
    vtkInteractionStyle
    vtkRenderingContextOpenGL2
    vtkRenderingCore
    vtkRenderingFreeType
    vtkRenderingGL2PSOpenGL2
    vtkRenderingOpenGL2
    vtkIOImage
    vtkImagingSources
)

file(GLOB my_SRC
    "*.c*"
)

add_executable(${PROJECT_NAME}
    ${my_SRC}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ../../include
    ../../3rd-party/ha_tracking/include
    ../../3rd-party/ha_tracking/3rd-party/ha_math/include
    ../../3rd-party/ha_tracking/3rd-party/ha_math/3rd-party/eigen-3.3.9
    ../../3rd-party/ha_tracking/3rd-party/ha_math/3rd-party/ha_util/include
    ${CMAKE_CURRENT_BINARY_DIR}/../../3rd-party   # include *.pb.h
    ${VTK_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${VTK_LIBRARIES}
    pthread
    ha_util
    ha_3dobj_tracking
    /usr/lib/x86_64-linux-gnu/libjpeg.so
)




################################################################################

#pragma once

#include <ha_3dobj_tracking/types.hh>

#include <map>
#include <mutex>
#include <deque>

#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkAutoInit.h>
#include <vtkPolyData.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkHexahedron.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkFloatArray.h>
#include <vtkCallbackCommand.h>
#include <vtkVectorText.h>
#include <vtkFollower.h>
#include <vtkImageActor.h>
#include <vtkImageImport.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkInteractorStyleTerrain.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

using namespace std;
using namespace ha::_3dobj_tracking;
// using namespace cimg_library;

struct frame_t{
    vector<Object<double>> dt_objs;
    vector<Object<double>> objs;
    PointCloudPtr<float> cloud;
    shared_ptr<uint8_t> img;
    // CImg<uint8_t> *img;
    // vtkNew<vtkJPEGReader> *img;
};

struct ViewPort{

    vtkNew<vtkRenderer> renderer_;
    std::array<double,3> pos_;
    std::array<double,3> focal_point_;
    int prallel_projection_;
    double view_angle_;
    // Camera settings
    vtkNew<vtkCamera> camera_;

    ViewPort(
            std::array<double,4> range,    // x_min, y_min, x_max, y_max
            std::array<double,3> position,  // x, y, z
            std::array<double,3> focal_point,    // x, y, z
            int prallel_projection,
            double view_angle
        );

    void resetCamera();
};

class ObjectGroup{
public:

    static constexpr size_t kBoxCapacity = 200;
    array<array<array<double,3>,8>,kBoxCapacity> box_points_coord_;
    array<array<vtkIdType,8>,kBoxCapacity> box_idxs_;
    size_t box_cnt_ = 0;
    vtkNew<vtkHexahedron> box_hexas_[kBoxCapacity];
    vtkNew<vtkPoints> box_points_;
    vtkNew<vtkUnstructuredGrid> box_ugrids_;
    vtkNew<vtkDataSetMapper> box_mapper_;
    vtkNew<vtkActor> box_actor_;

    vtkNew<vtkPolyDataMapper> label_mappers_[kBoxCapacity];
    vtkNew<vtkVectorText> label_vtexts_[kBoxCapacity];
    vtkNew<vtkFollower> label_actors_[kBoxCapacity];

    ObjectGroup();

    bool visibility_ = true;
    bool score_visibility_ = false;

    void update(vector<Object<double>> &frame);

    void setVisibility(bool flag);

};

class Viewer{
public:

    enum PlayStatus{
        STEP_FORWARD=0,
        CONTINUOUS_FORWARD,
        CONTINUOUS_FORWARDX2,
        CONTINUOUS_FORWARDX4,
        CONTINUOUS_FORWARDX8,

        STEP_BACKWARD,
        CONTINUOUS_BACKWARD,
        CONTINUOUS_BACKWARDX2,
        CONTINUOUS_BACKWARDX4,
        CONTINUOUS_BACKWARDX8,
        PLAY_STATUS_SIZE
    };

    enum ObjDisplayMode{
        TRACK_ONLY=0,
        DETECTION_ONLY,
        TRACK_AND_DETECTION
    };

    enum LabelMode{
        ID = 0,
        ID_SCORE
    };

    deque<frame_t> stream_;
    int data_pos_ = 0;
    mutex data_mtx_;

    map<uint64_t, PointCloudPtr<float>> clouds_;

    vtkNew<vtkNamedColors> colors_;

    // Points => PolyData => VertexGlyphFilter => PolyDataMapper => Actor
    vtkNew<vtkPoints> cloud_points_;
    vtkNew<vtkPolyData> cloud_poly_;
    vtkNew<vtkVertexGlyphFilter> cloud_filter_;
    vtkNew<vtkFloatArray> cloud_colors_;
    vtkNew<vtkPolyDataMapper> cloud_mapper_;
    vtkNew<vtkActor> cloud_actor_;
    vtkNew<vtkLookupTable> cloud_lut_;

    ObjectGroup track_obj_group_;

    ObjectGroup detection_obj_group_;

    vtkNew<vtkImageActor> img_actor_;
    vtkNew<vtkImageImport> img_import_;
    vtkNew<vtkImageCanvasSource2D> img_boxes_;

    vtkNew<vtkCallbackCommand> keypressCallback;
    std::vector<ViewPort*> views_;
    vtkNew<vtkRenderWindow> render_window_;
    vtkNew<vtkRenderWindowInteractor> interactor_;
    vtkNew<vtkInteractorStyleTerrain> interactor_style_;

    PlayStatus play_status_ = CONTINUOUS_FORWARD;

    ObjDisplayMode obj_disp_mode_ = TRACK_ONLY;

    LabelMode label_mode_ = ID;

    Viewer();

    // Load input from file in another thread.
    void startLoadInput(string track_folder, string detect_folder, string img_folder);

    // Listen input from network in antoher thread.
    void startListenInput(string folder);

    void run();

    void update();

private:
    void startLoadInputs(vector<string> box_paths, vector<string> pose_folders);

    vector<Object<double>> loadObjectsOfAFrame(string fpath);

    shared_ptr<uint8_t> loadAnImage(string fpath);

    std::vector<string> getDetectionBoxFpaths(string folder);

    std::vector<string> getImageFpaths(string folder);

    static void KeypressCallbackFunction(
        vtkObject* caller,
        long unsigned int eventId,
        void* clientData, void* callData);

    void drawImgBoxes(frame_t * frame);

};

class TimerHandler : public vtkCommand{
public:
    int timer_cnt_;
    Viewer * viewer_ = nullptr;
    TimerHandler() = default;

    static TimerHandler* New();

    virtual void Execute(
        vtkObject* caller,
        unsigned long eventId,
        void *vtkNotUsed(callData));

    void setViewer(Viewer *viewer);
};


################################################################################

#include "viewer.hh"
#include <regex>
#include <thread>
#include <Eigen/Dense>
#include <ha_util/core.h>
#include <jpeglib.h>

#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkCamera.h>
#include <vtkViewport.h>
#include <vtkImageMapper3D.h>
#include <vtkImageFlip.h>
#include <vtkImageBlend.h>

using namespace ha;
using namespace ha::_3dobj_tracking;
using namespace std;
using namespace Eigen;
namespace fs = boost::filesystem;

ViewPort::ViewPort(
    std::array<double,4> range,
    std::array<double,3> pos,
    std::array<double,3> focal_point,
    int prallel_projection,
    double view_angle){

    this->pos_ = pos;
    this->focal_point_ = focal_point;
    this->prallel_projection_ = prallel_projection;
    this->view_angle_ = view_angle;

    vtkNew<vtkNamedColors> colors;
    renderer_->SetViewport(range.data());
    renderer_->SetBackground(colors->GetColor3d("Black").GetData());
    resetCamera();
}

void ViewPort::resetCamera(){

    camera_->SetPosition(pos_.data());
    camera_->SetFocalPoint(focal_point_.data());
    camera_->SetParallelProjection(prallel_projection_);
    if(prallel_projection_){
        camera_->SetParallelScale(view_angle_);
        camera_->SetRoll(180);
    }
    else{
        camera_->SetViewAngle(view_angle_);
        camera_->SetViewUp(0,0,1);
    }
    renderer_->SetActiveCamera(camera_.Get());
}

TimerHandler* TimerHandler::New(){
    TimerHandler *handler = new TimerHandler();
    handler->timer_cnt_ = 0;
    return handler;
}

void TimerHandler::Execute(
    vtkObject* caller,
    unsigned long eventId,
    void *vtkNotUsed(callData)){

    if(viewer_->play_status_ != Viewer::STEP_FORWARD &&
        viewer_->play_status_ != Viewer::STEP_BACKWARD){
        viewer_->update();
    }

    timer_cnt_ ++;
}

void TimerHandler::setViewer(Viewer *viewer){
    viewer_ = viewer;
}

ObjectGroup::ObjectGroup(){

    for(int i=0; i<kBoxCapacity; i++){
        for(int j=0; j<8; j++){
            box_idxs_[i][j] = i * 8 + j;
        }
    }
    box_ugrids_->Allocate(kBoxCapacity);
    box_mapper_->SetInputData(box_ugrids_);
    box_actor_->SetMapper(box_mapper_);
    box_actor_->GetProperty()->EdgeVisibilityOn();
    box_actor_->GetProperty()->SetEdgeColor(255,255,100);
    box_actor_->GetProperty()->SetOpacity(0.3);

    for(int i=0; i<kBoxCapacity; i++){
        label_mappers_[i]->SetInputConnection(label_vtexts_[i]->GetOutputPort());
        label_actors_[i]->SetMapper(label_mappers_[i]);
        label_actors_[i]->SetScale(0.8,0.8,0.8);
    }
}

void ObjectGroup::update(vector<Object<double>> &objs){

    box_points_->Reset();
    box_ugrids_->Reset();
    box_actor_->SetVisibility(visibility_);
    for(int i=0; i<kBoxCapacity; i++){
        label_actors_[i]->VisibilityOff();
    }
    for(int i=0; i<objs.size(); i++){
        auto l2 = objs[i].lwh.x()/2;
        auto w2 = objs[i].lwh.y()/2;
        auto h2 = objs[i].lwh.z()/2;
        box_points_coord_[i][0] = {-l2,     -w2,    -h2};
        box_points_coord_[i][1] = {l2,      -w2,    -h2};
        box_points_coord_[i][2] = {l2,      w2,     -h2};
        box_points_coord_[i][3] = {-l2,     w2,     -h2};
        box_points_coord_[i][4] = {-l2,     -w2,    h2};
        box_points_coord_[i][5] = {l2,      -w2,    h2};
        box_points_coord_[i][6] = {l2,      w2,     h2};
        box_points_coord_[i][7] = {-l2,     w2,     h2};

        for(int j=0; j<8; j++){
            Matrix3d rot = objs[i].quat.toEigenQuat().toRotationMatrix();
            Eigen::Vector3d coord(box_points_coord_[i][j].data());
            coord = rot * coord + objs[i].pos.toEigen();
            box_points_coord_[i][j] = {coord.x(), coord.y(), coord.z()};
        }

        for(int j=0; j<8; j++){
            box_points_->InsertPoint(i*8+j, box_points_coord_[i][j].data());
        }
        box_ugrids_->InsertNextCell(VTK_HEXAHEDRON, 8, box_idxs_[i].data());

        string label_str = fmt2str("i%d",objs[i].id);
        label_str += score_visibility_ ? fmt2str("s:%.2f", objs[i].score) : "";
        label_vtexts_[i]->SetText(label_str.c_str());
        label_vtexts_[i]->Update();
        label_actors_[i]->SetPosition(box_points_coord_[i][0][0], box_points_coord_[i][0][1], box_points_coord_[i][0][2]);
        if(visibility_){
            label_actors_[i]->VisibilityOn();
        }
    }
    box_ugrids_->SetPoints(box_points_);
}

void ObjectGroup::setVisibility(bool flag){
    visibility_ = flag;
}

Viewer::Viewer(){

    LocalClock::init();

    // Point cloud renderring pipeline.
    cloud_lut_->SetNumberOfColors(256);
    cloud_lut_->SetHueRange(0.667,0);
    cloud_lut_->Build();
    cloud_filter_->SetInputData(cloud_poly_.Get());
    cloud_mapper_->SetInputConnection(cloud_filter_->GetOutputPort());
    cloud_mapper_->SetScalarRange(0,255);
    cloud_mapper_->SetLookupTable(cloud_lut_);
    cloud_actor_->SetMapper(cloud_mapper_.Get());

    track_obj_group_.box_actor_->GetProperty()->SetColor(10,10,0);
    detection_obj_group_.box_actor_->GetProperty()->SetColor(5,0,0);
    for(int i=0; i<ObjectGroup::kBoxCapacity; i++){
        track_obj_group_.label_actors_[i]->GetProperty()->SetColor(10,10,0);
        detection_obj_group_.label_actors_[i]->GetProperty()->SetColor(10,1,0);
    }

    int cols = 960, rows = 540;
    img_import_->SetDataSpacing(1, 1, 1);
    img_import_->SetDataOrigin(0, 0, 0);
    img_import_->SetWholeExtent(0, cols-1, 0, rows-1, 0, 0);
    img_import_->SetDataExtentToWholeExtent();
    img_import_->SetDataScalarTypeToUnsignedChar();
    img_import_->SetNumberOfScalarComponents(3);
    vtkNew<vtkImageFlip> fliper;
    fliper->SetFilteredAxes(0);
    fliper->SetInputData(img_import_->GetOutput());
    fliper->Update();
    img_boxes_->SetNumberOfScalarComponents(3);
    img_boxes_->SetScalarTypeToUnsignedChar();
    img_boxes_->SetExtent(img_import_->GetDataExtent());
    vtkNew<vtkImageBlend> blend;
    blend->AddInputConnection(fliper->GetOutputPort());
    blend->AddInputConnection(img_boxes_->GetOutputPort());
    blend->SetOpacity(0, 0.6);
    blend->SetOpacity(1, 0.4);
    blend->SetBlendModeToCompound();
    img_actor_->GetMapper()->SetInputConnection(blend->GetOutputPort());

    // vtkNew<vtkRenderer> is noncopyable, so it's mother i.e., 
    // ViewPort is noncopyable too. We can only capture their pointers into
    // vector container.
    double b = 0;
    ViewPort *v0 = new ViewPort({0,0,0.5-b,1}, {0,0,5}, {0,0,0}, 1, 90);
    ViewPort *v1 = new ViewPort({0.5+b,0,1,0.5-b}, {0,5,5}, {0,-5,0}, 0, 60);
    ViewPort *v2 = new ViewPort({0.5+b,0.5+b,1,1}, {480,270,5}, {480,270,0}, 1, 275);
    // ViewPort v3({0.5+b,0.5+b,1,1}, {0,0,0}, {0,-1,0}, 0, 45, cloud_actor_, map_actor_);

    views_ = {v0, v1};
    for(auto &v : views_){
        v->renderer_->AddActor(track_obj_group_.box_actor_);
        v->renderer_->AddActor(detection_obj_group_.box_actor_);
        v->renderer_->AddActor(cloud_actor_);
        for(int i=0; i<ObjectGroup::kBoxCapacity; i++){
            v->renderer_->AddViewProp(detection_obj_group_.label_actors_[i]);
            v->renderer_->AddViewProp(track_obj_group_.label_actors_[i]);
        }
        render_window_->AddRenderer(v->renderer_); 
    }

    v2->renderer_->AddActor(img_actor_);
 
    render_window_->AddRenderer(v2->renderer_);

    keypressCallback->SetCallback(KeypressCallbackFunction);
    keypressCallback->SetClientData((void*)this);
    interactor_->AddObserver(vtkCommand::KeyPressEvent, keypressCallback);
    interactor_->SetRenderWindow(render_window_);
    interactor_->Initialize();
    interactor_->SetInteractorStyle(interactor_style_);
    render_window_->SetWindowName("Tracking");
    render_window_->SetSize(1920,1080);

}

void Viewer::run(){

    vtkNew<TimerHandler> cb;
    cb->setViewer(this);
    interactor_->AddObserver(vtkCommand::TimerEvent, cb);
    int timer_id = interactor_->CreateRepeatingTimer(50);
    interactor_->Start();
}

void Viewer::update(){

    frame_t frame;
    data_mtx_.lock();
    constexpr int kPlaySteps[PLAY_STATUS_SIZE] = {1,1,2,4,8,1,1,2,4,8};
    if(!stream_.empty()){
        frame = stream_[data_pos_];
        int step = kPlaySteps[play_status_];
        if(play_status_ >= STEP_FORWARD && play_status_ <= CONTINUOUS_FORWARDX8){
            if(data_pos_ + step > stream_.size()-1){
                data_pos_ = stream_.size()-1;
            }
            else{
                data_pos_ += step;
            }
        }
        else if(play_status_ >= STEP_BACKWARD && play_status_ <= CONTINUOUS_BACKWARDX8){
            if(data_pos_ - step < 0){
                data_pos_ = 0;
            }
            else{
                data_pos_ -= step;
            }
        }
    }
    data_mtx_.unlock();

    auto t_beg = LocalClock::getTimestampUS();
    track_obj_group_.update(frame.objs);
    detection_obj_group_.update(frame.dt_objs);
    auto t_end = LocalClock::getTimestampUS();
    // printf("Inserting data cost: %lu ms\n", (t_end-t_beg)/1000);

    vtkNew<vtkNamedColors> named_colors;
    cloud_colors_->Reset();
    cloud_colors_->SetNumberOfComponents(1);
    cloud_colors_->SetName("colors");
    cloud_points_->Reset();
    frame.cloud->iterate([this,&named_colors](size_t idx, PointItf<float> pt){
        cloud_points_->InsertNextPoint(pt.x(), pt.y(), pt.z());
        cloud_colors_->InsertNextValue(pt.i());
    });
    cloud_poly_->SetPoints(cloud_points_);
    cloud_poly_->GetPointData()->SetScalars(cloud_colors_);
    cloud_filter_->Update();
    // printf("[vtkRenderer]: cloud cnt:%llu\n", cloud_points_->GetNumberOfPoints());

    drawImgBoxes(&frame);
    img_import_->SetImportVoidPointer(frame.img.get());
    img_import_->Update();
    
    img_actor_->Update();

    render_window_->Render();
}

void Viewer::drawImgBoxes(frame_t * frame){

    uint32_t cols = img_import_->GetDataExtent()[1]+1;
    uint32_t rows = img_import_->GetDataExtent()[3]+1;
    img_boxes_->SetDrawColor(0,0,0);
    img_boxes_->FillBox(img_import_->GetDataExtent()[0], img_import_->GetDataExtent()[1],
                    img_import_->GetDataExtent()[2], img_import_->GetDataExtent()[3]);

    Eigen::Quaterniond q_l2c(0.016213200031258722, 0.0030578899383849464, 0.7114721800418571,-0.7025205466606356);

    Matrix3d rot_l2c = q_l2c.toRotationMatrix().block<3,3>(0,0);
    Eigen::Vector3d p_l2c;
    p_l2c << 0.0002585796504896516, -0.03907777167811011, -0.0440125762408362;

    Matrix<double,2,3> K;
    K << 1970.0131/2, 0, 970.0002/2, 0, 1970.0091/2, 483.2988/2;

    img_boxes_->SetDrawColor(1000,1000,0);
    int idxes[12][2] = {{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7}};
    for(int i=0; i<frame->objs.size(); i++){
        auto l2 = frame->objs[i].lwh.x()/2;
        auto w2 = frame->objs[i].lwh.y()/2;
        auto h2 = frame->objs[i].lwh.z()/2;
        array<Eigen::Vector3d,8> pts;
        pts[0] = {-l2,     -w2,    -h2};
        pts[1] = {l2,      -w2,    -h2};
        pts[2] = {l2,      w2,     -h2};
        pts[3] = {-l2,     w2,     -h2};
        pts[4] = {-l2,     -w2,    h2};
        pts[5] = {l2,      -w2,    h2};
        pts[6] = {l2,      w2,     h2};
        pts[7] = {-l2,     w2,     h2};

        array<Eigen::Vector2d,8> m;
        Matrix3d rot = frame->objs[i].quat.toEigenQuat().toRotationMatrix();
        Vector3d p = frame->objs[i].pos.toEigen();
        for(int j=0; j<8; j++){
            pts[j] = rot * pts[j] + p;
            pts[j] = rot_l2c * pts[j] + p_l2c;
            if(pts[j].z() > 0){
                m[j] = K * pts[j] / pts[j].z();
                m[j].x() = cols - m[j].x();
            }
            else {
                m[j] = {9999,9999};
            }
        }
        for(int j=0; j<12; j++){
            img_boxes_->DrawSegment(m[idxes[j][0]].x(), m[idxes[j][0]].y(), m[idxes[j][1]].x(), m[idxes[j][1]].y());
        }
    }
}

vector<Object<double>> Viewer::loadObjectsOfAFrame(string fpath){
    ifstream ifs(fpath);
    if(!ifs.is_open()){
        throw runtime_error("can't open file to read: "+ fpath);
    }
    ifs.seekg(0, ios::end);
    size_t len = ifs.tellg();
    ifs.seekg(0, ios::beg);
    auto buf = shared_ptr<char>(new char[len]);
    ifs.read(buf.get(), len);

    auto frame = ha::_3dobj_tracking::Frame<double>::fromProtobuf(buf, len);

    return frame.objs_;
}

shared_ptr<uint8_t> Viewer::loadAnImage(string fpath){
    struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	JSAMPROW raw_line[1];
	
	FILE *infile = fopen( fpath.c_str(), "rb" );
	unsigned long location = 0;
	int i = 0;
	
	if ( !infile ) {
        throw runtime_error("Can't open file to read: "+fpath);
	}

	cinfo.err = jpeg_std_error( &jerr );
	jpeg_create_decompress( &cinfo );
	jpeg_stdio_src( &cinfo, infile );
	jpeg_read_header( &cinfo, TRUE );

	jpeg_start_decompress( &cinfo );

    uint32_t cols = cinfo.output_width / 2;
    uint32_t rows = cinfo.output_height / 2;
    uint32_t channel = cinfo.num_components;
	auto image = shared_ptr<uint8_t>(new uint8_t[cols*rows*cinfo.num_components] );

	raw_line[0] = (uint8_t *)malloc( cinfo.output_width*cinfo.num_components );

    for(int r=0; r<rows; r++){
        uint8_t * line = image.get() + r * cols * channel;
        jpeg_read_scanlines(&cinfo, raw_line, 1);
        
        for(int c=0; c<cols; c++){
            memcpy(line+c*channel, raw_line[0]+c*channel*2, channel);
        }
        jpeg_skip_scanlines(&cinfo, 1);
    }

	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress( &cinfo );
	free( raw_line[0] );
	fclose( infile );

	return image;
}

std::vector<string> Viewer::getDetectionBoxFpaths(string folder){

    std::vector<std::string> fpaths;
    std::set<int> idxs;
    for(auto &p : fs::directory_iterator(folder)){
        if(boost::filesystem::is_directory(p)){
            continue;
        }
        smatch m_idx;
        if(regex_search(p.path().string(), m_idx, regex("/([0-9]+).bin"))){
            idxs.insert(atoi(m_idx[1].str().c_str()));
        }
    }
    for(auto i : idxs){
        fpaths.push_back(fmt2str("%s/%d.bin",folder.c_str(),i));
    }

    return fpaths;
}

vector<string> Viewer::getImageFpaths(string folder){
    auto sub_folders = list_ordered_folders(folder);
    vector<string> fpaths; 
    for(auto sub_folder : sub_folders){
        auto files = list_ordered_files(sub_folder);
        for(auto file : files){
            smatch match_res;
            if(regex_search(file, match_res, regex(".+.jpg"))){
                string path = match_res[0].str();
                fpaths.push_back(path);
            }
        }
    }
    return fpaths;
}

void Viewer::startLoadInput(string track_folder, string detect_folder, string img_folder){
    auto dt_box_fpaths = getDetectionBoxFpaths(detect_folder);
    auto box_fpaths = list_ordered_files(track_folder+"/box");
    auto cloud_fpaths = list_ordered_files(track_folder+"/cloud");
    auto img_fpaths = getImageFpaths(img_folder);
    thread th([dt_box_fpaths, box_fpaths,cloud_fpaths, img_fpaths,this](){
        for(size_t i=0; i<box_fpaths.size(); i++){

            // Load detections of a frame.
            auto objs = loadObjectsOfAFrame(box_fpaths[i]);

            auto dt_objs = loadObjectsOfAFrame(dt_box_fpaths[i]);

            // Load point cloud of a frame.
            auto cld = make_shared<PointCloud<float>>();
            cld->load(cloud_fpaths[i]);

            auto img = loadAnImage(img_fpaths[i]);

            frame_t d = {dt_objs, objs, cld, img};

            data_mtx_.lock();
            stream_.push_back(d);
            data_mtx_.unlock();

            // printf("[LD] complete a frame, total:%lu\n", i+1);

        }
    });
    th.detach();
}

void Viewer::KeypressCallbackFunction(vtkObject* caller,
                              long unsigned int vtkNotUsed(eventId),
                              void* clientData,
                              void* vtkNotUsed(callData)) {
//   std::cout << "Keypress callback" << std::endl;

    Viewer * me = (Viewer*)clientData;

    vtkRenderWindowInteractor* iren =
      static_cast<vtkRenderWindowInteractor*>(caller);
    
    if(iren->GetKeyCode() == 100){              // "d"
    }
    else if(iren->GetKeyCode() == 102){         // "f"
        if(me->play_status_ == CONTINUOUS_FORWARD){
            me->play_status_ = CONTINUOUS_FORWARDX2;
        }
        else if(me->play_status_ == CONTINUOUS_FORWARDX2){
            me->play_status_ = CONTINUOUS_FORWARDX4;
        }
        else if(me->play_status_ == CONTINUOUS_FORWARDX4 ||
                me->play_status_ == CONTINUOUS_FORWARDX8){
            me->play_status_ = CONTINUOUS_FORWARDX8;
        }
        else{
            me->play_status_ = CONTINUOUS_FORWARD;
        }
    }
    else if(iren->GetKeyCode() == 115){         // "s"
        me->play_status_ = STEP_BACKWARD;
    }
    else if(iren->GetKeyCode() == 97){          // "a"
        if(me->play_status_ == CONTINUOUS_BACKWARD){
            me->play_status_ = CONTINUOUS_BACKWARDX2;
        }
        else if(me->play_status_ == CONTINUOUS_BACKWARDX2){
            me->play_status_ = CONTINUOUS_BACKWARDX4;
        }
        else if(me->play_status_ == CONTINUOUS_BACKWARDX4 ||
                me->play_status_ == CONTINUOUS_BACKWARDX8){
            me->play_status_ = CONTINUOUS_BACKWARDX8;
        }
        else{
            me->play_status_ = CONTINUOUS_BACKWARD;
        }
    }
    else if(iren->GetKeyCode() == 103){     // "g"
        for(auto v: me->views_){
            v->resetCamera();
        }
    }
    else if(iren->GetKeyCode() == 104)  {   // "h"
        if(me->obj_disp_mode_ == TRACK_ONLY){
            me->obj_disp_mode_ = DETECTION_ONLY;
            me->track_obj_group_.setVisibility(false);
            me->detection_obj_group_.setVisibility(true);
        }
        else if(me->obj_disp_mode_ == DETECTION_ONLY){
            me->obj_disp_mode_ = TRACK_AND_DETECTION;
            me->track_obj_group_.setVisibility(true);
            me->detection_obj_group_.setVisibility(true);
        }
        else{ // me->obj_disp_mode_ == TRACK_AND_DETECTION
            me->obj_disp_mode_ = TRACK_ONLY;
            me->track_obj_group_.setVisibility(true);
            me->detection_obj_group_.setVisibility(false);
        }
    }
    else if(iren->GetKeyCode() == 106)  {   // "j"
        if(me->label_mode_ == ID){
            me->label_mode_ = ID_SCORE;
            me->track_obj_group_.score_visibility_ = true;
            me->detection_obj_group_.score_visibility_ = true;
        }
        else{
            me->label_mode_ = ID;
            me->track_obj_group_.score_visibility_ = false;
            me->detection_obj_group_.score_visibility_ = false;
        }
    }
    me->update();

    // std::cout << "Pressed: " << iren->GetKeySym() << std::endl;
}