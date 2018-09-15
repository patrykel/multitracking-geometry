import FWCore.ParameterSet.Config as cms
process = cms.Process("GeometryInfo")

# minimum of logs
process.load("Configuration.TotemCommon.LoggerMin_cfi")

# geometry
process.load("Configuration.TotemCommon.geometryRP_cfi")
process.XMLIdealGeometryESSource.geomXMLFiles.append("Geometry/TotemRPData/data/2015_10_17_fill4510/RP_Dist_Beam_Cent.xml")
# process.XMLIdealGeometryESSource.geomXMLFiles.append('Geometry/TotemRPData/data/RP_Garage/RP_Dist_Beam_Cent.xml')

# alignment
process.load("TotemAlignment.RPDataFormats.TotemRPIncludeAlignments_cfi")
process.TotemRPIncludeAlignments.RealFiles = cms.vstring(
'/afs/cern.ch/exp/totem/scratch/Release/raw_data_reco/rev11326/CMSSW_7_0_4/src/TotemAlignment/RPData/LHC/2015_10_17_fill4510/version2/sr+el/45.xml',
'/afs/cern.ch/exp/totem/scratch/Release/raw_data_reco/rev11326/CMSSW_7_0_4/src/TotemAlignment/RPData/LHC/2015_10_17_fill4510/version2/sr+el/56.xml'
)



# no events to process
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.GeomInfo = cms.EDAnalyzer("GeometryInfoModule")

process.p = cms.Path(
    process.GeomInfo
)
