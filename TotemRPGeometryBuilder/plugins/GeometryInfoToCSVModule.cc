/****************************************************************************
*
* supporting module for TOTEM offline software.
* Authors: 
*  Jan Kašpar (jan.kaspar@gmail.com) 
*  Anna Szczepanska (anna.szczepanska.krk@gmail.com) 
*    
****************************************************************************/

#include <fstream>

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESWatcher.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TotemRecords/interface/RealGeometryRecord.h"
#include "Geometry/TotemRecords/interface/MisalignedGeometryRecord.h"
#include "Geometry/TotemRecords/interface/MeasuredGeometryRecord.h"

#include "Geometry/TotemRPGeometryBuilder/interface/TotemRPGeometry.h"
#include "DataFormats/TotemRPDetId/interface/TotRPDetId.h"


//----------------------------------------------------------------------------------------------------

/**
 * \ingroup TotemRPGeometry
 * \brief Class to print out information on current geometry.
 *
 * See schema of \ref TotemRPGeometry "TOTEM RP geometry classes"
 **/
class GeometryInfoToCSVModule : public edm::EDAnalyzer
{
  public:
    explicit GeometryInfoToCSVModule(const edm::ParameterSet&);
    ~GeometryInfoToCSVModule();

    struct MeanRPData
    {
      unsigned int N, N_u, N_v;
      double Sx, Sy, Sz, Sdx_u, Sdx_v, Sdy_u, Sdy_v;
    
      MeanRPData() : N(0), N_u(0), N_v(0), Sx(0.), Sy(0.), Sz(0.), Sdx_u(0.), Sdx_v(0.), Sdy_u(0.), Sdy_v(0.) {}

      void Fill(double x, double y, double z, bool uDir, double dx, double dy)
      {
        N++;
        Sx += x;
        Sy += y;
        Sz += z;

        if (uDir)
        {
          N_u++;
          Sdx_u += dx;
          Sdy_u += dy;
        } else {
          N_v++;
          Sdx_v += dx;
          Sdy_v += dy;
        }
      }
    };

  private: 
    ofstream rpsFile;
    ofstream sensorsFile;
    ofstream averageFile;

    std::string geometryType;

    bool printRPInfo, printSensorInfo, printMeanSensorInfo;

    edm::ESWatcher<IdealGeometryRecord> watcherIdealGeometry;
    edm::ESWatcher<MeasuredGeometryRecord> watcherMeasuredGeometry;
    edm::ESWatcher<RealGeometryRecord> watcherRealGeometry;
    edm::ESWatcher<MisalignedGeometryRecord> watcherMisalignedGeometry;

    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    void PrintGeometry(const TotemRPGeometry &, const edm::Event&);
};

using namespace edm;
using namespace std;


//----------------------------------------------------------------------------------------------------

GeometryInfoToCSVModule::GeometryInfoToCSVModule(const edm::ParameterSet& ps) :
  geometryType(ps.getUntrackedParameter<string>("geometryType", "real")),
  printRPInfo(ps.getUntrackedParameter<bool>("printRPInfo", true)),
  printSensorInfo(ps.getUntrackedParameter<bool>("printSensorInfo", true)),
  printMeanSensorInfo(ps.getUntrackedParameter<bool>("printMeanSensorInfo", true))
{
	std::string fillFileName = ps.getParameter<string>("fill_name");
    rpsFile.open(fillFileName + "_rps.csv");
    sensorsFile.open(fillFileName + "_sensors.csv");
    averageFile.open(fillFileName + "_average.csv");
}

//----------------------------------------------------------------------------------------------------

GeometryInfoToCSVModule::~GeometryInfoToCSVModule()
{
  rpsFile.close();
  sensorsFile.close();
  averageFile.close();
}

//----------------------------------------------------------------------------------------------------

void GeometryInfoToCSVModule::beginRun(edm::Run const&, edm::EventSetup const& iSetup)
{
}

//----------------------------------------------------------------------------------------------------

void GeometryInfoToCSVModule::endJob()
{
}

//----------------------------------------------------------------------------------------------------

void GeometryInfoToCSVModule::analyze(const edm::Event& event, const edm::EventSetup& es)
{
  ESHandle<TotemRPGeometry> geometry;

  if (!geometryType.compare("ideal"))
  {
    if (watcherIdealGeometry.check(es))
    {
      es.get<IdealGeometryRecord>().get(geometry);
      PrintGeometry(*geometry, event);
    }
    return;
  }

  if (!geometryType.compare("measured"))
  {
    if (watcherMeasuredGeometry.check(es))
    {
      es.get<MeasuredGeometryRecord>().get(geometry);
      PrintGeometry(*geometry, event);
    }
    return;
  }

  if (!geometryType.compare("real"))
  {
    if (watcherRealGeometry.check(es))
    {
      es.get<RealGeometryRecord>().get(geometry);
      PrintGeometry(*geometry, event);
    }
    return;
  }

  if (!geometryType.compare("misaligned"))
  {
    if (watcherMisalignedGeometry.check(es))
    {
      es.get<MisalignedGeometryRecord>().get(geometry);
      PrintGeometry(*geometry, event);
    }
    return;
  }

  throw cms::Exception("GeometryInfoToCSVModule") << "Unknown geometry type: `" << geometryType << "'.";
}

//----------------------------------------------------------------------------------------------------

void GeometryInfoToCSVModule::PrintGeometry(const TotemRPGeometry &geometry, const edm::Event& event)
{
  time_t unixTime = event.time().unixTime();
  char timeStr[50];
  strftime(timeStr, 50, "%F %T", localtime(&unixTime));
  printf(">> GeometryInfoToCSVModule::PrintGeometry\n\tnew %s geometry found in run=%u, event=%u, UNIX timestamp=%lu (%s)\n",
    geometryType.c_str(), event.id().run(), event.id().event(), unixTime, timeStr);

  // RP geometry
  if (printRPInfo)
  {
    printf("\n* RPs:\n");
    printf("  ID |   x (mm)    |   y (mm)    |   z  (m)  |\n");
    rpsFile << "ID,x,y,z\n";
    for (TotemRPGeometry::RPDeviceMapType::const_iterator it = geometry.beginRP(); it != geometry.endRP(); ++it)
    {
      const DDTranslation &t = it->second->translation();
      printf(" %3i | %+11.3f | %+11.3f | %+9.4f |\n", it->first, t.x(), t.y(), t.z() * 1E-3);
      rpsFile << it->first << "," << t.x() << "," << t.y() << "," << t.z() * 1E-3 << "\n";
    }
  }
  
  // sensor geometry
  if (printSensorInfo)
  {
    printf("\n* silicon sensors:\n");
    printf("DetId |            detector center           |  readout direction  |\n");
    printf("      |   x (mm)   |   y (mm)   |   z  (m)   |    dx    |    dy    |\n");
    sensorsFile << "DetId,x,y,z,dx,dy\n";
    for (TotemRPGeometry::mapType::const_iterator it = geometry.beginDet(); it != geometry.endDet(); ++it)
    {
      TotRPDetId id(it->first);
  
      double x = it->second->translation().x();
      double y = it->second->translation().y();
      double z = it->second->translation().z() * 1E-3;
  
      double dx = 0., dy = 0.;
      geometry.GetReadoutDirection(it->first, dx, dy);
  
      printf("%4i  |  %8.3f  |  %8.3f  |  %9.4f | %8.3f | %8.3f |\n", id.DetectorDecId(), x, y, z, dx, dy);
      sensorsFile << id.DetectorDecId() << "," << x << "," << y << "," << z << "," << dx << "," << dy << "\n";
    }
  }
  
  // sensor geometry averaged over 1 RP
  if (printMeanSensorInfo)
  {
    printf("\n* average over silicon sensors of 1 RP:\n");

    map<unsigned int, MeanRPData> data; // map: RP Id --> sums of geometrical info

    for (TotemRPGeometry::mapType::const_iterator it = geometry.beginDet(); it != geometry.endDet(); ++it)
    {
      unsigned int decId = TotRPDetId::RawToDecId(it->first);
      unsigned int rpId = decId / 10;
      bool uDirection = TotRPDetId::IsStripsCoordinateUDirection(decId);
  
      double x = it->second->translation().x();
      double y = it->second->translation().y();
      double z = it->second->translation().z() * 1E-3;
  
      double dx = 0., dy = 0.;
      geometry.GetReadoutDirection(it->first, dx, dy);
  
      data[rpId].Fill(x, y, z, uDirection, dx, dy);
    }

    printf("RPId |                center                |      U direction    |      V direction    |\n");
    printf("     |   x (mm)   |   y (mm)   |   z  (m)   |    dx    |    dy    |    dx    |    dy    |\n");
    averageFile << "RPId,x,y,z,udx,udy,vdx,vdy\n";
    
    for (map<unsigned int, MeanRPData>::iterator it = data.begin(); it != data.end(); ++it)
    {
      const MeanRPData &d = it->second;
  
      double mx = (d.N > 0) ? d.Sx / d.N : 0.;
      double my = (d.N > 0) ? d.Sy / d.N : 0.;
      double mz = (d.N > 0) ? d.Sz / d.N : 0.;

      double mdx_u = (d.N_u > 0) ? d.Sdx_u / d.N_u : 0.;
      double mdy_u = (d.N_u > 0) ? d.Sdy_u / d.N_u : 0.;

      double mdx_v = (d.N_v > 0) ? d.Sdx_v / d.N_v : 0.;
      double mdy_v = (d.N_v > 0) ? d.Sdy_v / d.N_v : 0.;

      
      printf(" %3i |  %8.3f  |  %8.3f  |  %9.4f | %8.3f | %8.3f | %8.3f | %8.3f |\n", it->first, mx, my, mz, mdx_u, mdy_u, mdx_v, mdy_v);
      averageFile << it->first << "," << mx << "," << my << "," << mz << "," << mdx_u << "," << mdy_u << "," << mdx_v << "," << mdy_v << "\n";
    }
  }
}

DEFINE_FWK_MODULE(GeometryInfoToCSVModule);

