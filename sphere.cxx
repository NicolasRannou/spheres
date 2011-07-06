// vtk stuff
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkStripper.h>

// std
#include <list>
#include <iostream>

int main(int argc, char ** argv)
{

  int numberOfSpheres = 2;

  if ( argc != 2 )
    {
    std::cout << "8 Spheres (2x2x2)" << std::endl;    
    }
  else
    {
    int number = atoi(argv[1])*atoi(argv[1])*atoi(argv[1]);
    std::cout << number << " Spheres (" << argv[1] << "x" << argv[1] << "x" << argv[1] << ")" << std::endl;    
    numberOfSpheres = atoi(argv[1]);
    }
  
  // smartpointers for ease
  vtkSmartPointer<vtkRenderer> renderer = 
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
  
  std::list<vtkPolyData*> polydatas;
  std::list<vtkPolyDataMapper*> mappers;
  std::list<vtkActor*> actors;

  // no more smartpointers to control everything...
  // Create a sphere
  vtkSphereSource* sphereSource = vtkSphereSource::New();
  sphereSource->SetRadius(0.3);
  
  for(int i=0;i<numberOfSpheres; ++i )
    { 
    for(int j=0; j<numberOfSpheres; ++j)
      {
      for(int k=0; k<numberOfSpheres; ++k)
        {
        sphereSource->SetCenter(i, j, k);
        sphereSource->Update();
        
        // we work on a polydata to be in a minimalistic case
        vtkPolyData* polydata = vtkPolyData::New();
        polydata->DeepCopy(sphereSource->GetOutput());
        polydatas.push_back(polydata);

        vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
        mapper->SetInput(polydata);
        mapper->Update();
        mappers.push_back(mapper);
        
        vtkActor* actor = vtkActor::New();
        actor->SetMapper(mapper);
        actors.push_back(actor);

        renderer->AddActor(actor);
        }
      } 
    }

  sphereSource->Delete();
  std::cout << "delete sphere source ref count: " << sphereSource->GetReferenceCount() << std::endl;;

  // render and start interaction so we can check status of memory
  renderWindow->Render();
 // renderWindowInteractor->Start();

  // remove actors from visu
  std::list<vtkActor*>::iterator actor_iterator = actors.begin();
  
  while( actor_iterator != actors.end() )
    {
    renderer->RemoveActor(*actor_iterator);
    ++actor_iterator;
    }

  // delete actors and polydatas
  actor_iterator = actors.begin();
  std::list<vtkPolyData*>::iterator poly_iterator = polydatas.begin();
  std::list<vtkPolyDataMapper*>::iterator map_iterator = mappers.begin();

  while( poly_iterator != polydatas.end() )
    {
    (*actor_iterator)->Delete();
    std::cout << "delete actor ref count: " << (*actor_iterator)->GetReferenceCount() << std::endl;;
    (*map_iterator)->Delete();
    std::cout << "delete map ref count: " << (*map_iterator)->GetReferenceCount() << std::endl;;
    (*poly_iterator)->Delete();
    std::cout << "delete poly ref count: " << (*poly_iterator)->GetReferenceCount() << std::endl;;
    
    ++poly_iterator;
    ++map_iterator;
    ++actor_iterator;
    }

  // render and start interaction so we can check status of memory
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
