import FWCore.ParameterSet.Config as cms
process = cms.Process("GeometryInfoToCSV")

# Fill numbers:
# 4499, 4505, 4509, 4510, 4511

# minimum of logs
process.load("Configuration.TotemCommon.LoggerMin_cfi")

# geometry
process.load("Configuration.TotemCommon.geometryRP_cfi")

# RP_Dist_Beam_Cent.xml 

# process.XMLIdealGeometryESSource.geomXMLFiles.append("Geometry/TotemRPData/data/2015_10_15_fill4499/RP_Dist_Beam_Cent.xml")
# process.XMLIdealGeometryESSource.geomXMLFiles.append("Geometry/TotemRPData/data/2015_10_16_fill4505/RP_Dist_Beam_Cent.xml")
# process.XMLIdealGeometryESSource.geomXMLFiles.append("Geometry/TotemRPData/data/2015_10_17_fill4509/RP_Dist_Beam_Cent.xml")
# process.XMLIdealGeometryESSource.geomXMLFiles.append("Geometry/TotemRPData/data/2015_10_17_fill4510/RP_Dist_Beam_Cent.xml")
process.XMLIdealGeometryESSource.geomXMLFiles.append("Geometry/TotemRPData/data/2015_10_18_fill4511/RP_Dist_Beam_Cent.xml")

# process.XMLIdealGeometryESSource.geomXMLFiles.append('Geometry/TotemRPData/data/RP_Garage/RP_Dist_Beam_Cent.xml')

# alignment
process.load("TotemAlignment.RPDataFormats.TotemRPIncludeAlignments_cfi")
process.TotemRPIncludeAlignments.RealFiles = cms.vstring(


# '/afs/cern.ch/exp/totem/scratch/Release/raw_data_reco/rev11326/CMSSW_7_0_4/src/TotemAlignment/RPData/LHC/2015_10_15_fill4499/version2/sr+el/45.xml',
# '/afs/cern.ch/exp/totem/scratch/Release/raw_data_reco/rev11326/CMSSW_7_0_4/src/TotemAlignment/RPData/LHC/2015_10_15_fill4499/version2/sr+el/56.xml'

# '/afs/cern.ch/exp/totem/scratch/Release/raw_data_reco/rev11326/CMSSW_7_0_4/src/TotemAlignment/RPData/LHC/2015_10_16_fill4505/version2/sr+el/45.xml',
# '/afs/cern.ch/exp/totem/scratch/Release/raw_data_reco/rev11326/CMSSW_7_0_4/src/TotemAlignment/RPData/LHC/2015_10_16_fill4505/version2/sr+el/56.xml'

# '/afs/cern.ch/exp/totem/scratch/Release/raw_data_reco/rev11326/CMSSW_7_0_4/src/TotemAlignment/RPData/LHC/2015_10_17_fill4509/version2/sr+el/45.xml',
# '/afs/cern.ch/exp/totem/scratch/Release/raw_data_reco/rev11326/CMSSW_7_0_4/src/TotemAlignment/RPData/LHC/2015_10_17_fill4509/version2/sr+el/56.xml'

# '/afs/cern.ch/exp/totem/scratch/Release/raw_data_reco/rev11326/CMSSW_7_0_4/src/TotemAlignment/RPData/LHC/2015_10_17_fill4510/version2/sr+el/45.xml',
# '/afs/cern.ch/exp/totem/scratch/Release/raw_data_reco/rev11326/CMSSW_7_0_4/src/TotemAlignment/RPData/LHC/2015_10_17_fill4510/version2/sr+el/56.xml'

'/afs/cern.ch/exp/totem/scratch/Release/raw_data_reco/rev11326/CMSSW_7_0_4/src/TotemAlignment/RPData/LHC/2015_10_18_fill4511/version2/sr+el/45.xml',
'/afs/cern.ch/exp/totem/scratch/Release/raw_data_reco/rev11326/CMSSW_7_0_4/src/TotemAlignment/RPData/LHC/2015_10_18_fill4511/version2/sr+el/56.xml'
)


# no events to process
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.GeomInfo = cms.EDAnalyzer("GeometryInfoToCSVModule", 
	fill_name = cms.string('2015_10_18_fill4511')
	)

process.p = cms.Path(
    process.GeomInfo
)
