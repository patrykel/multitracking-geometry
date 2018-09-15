#
#Author Seyed Mohsen Etesami
# 23/06/2015


import FWCore.ParameterSet.Config as cms

process = cms.Process("DUMP")
# geometry configuration
process.load("Configuration.TotemCommon.geometryRP_cfi")
process.XMLIdealGeometryESSource.geomXMLFiles.append('Geometry/TotemRPData/data/RP_Beta_90/RP_Dist_Beam_Cent.xml')
process.XMLIdealGeometryESSource.rootNodeName = cms.string('cms:OCMS')
if 'Geometry/CMSCommonData/data/normal/cmsextent.xml' in process.XMLIdealGeometryESSource.geomXMLFiles:
    process.XMLIdealGeometryESSource.geomXMLFiles.remove('Geometry/CMSCommonData/data/normal/cmsextent.xml')
    process.XMLIdealGeometryESSource.geomXMLFiles.append('Geometry/CMSCommonData/data/extend/cmsextent.xml')


process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger = cms.Service("MessageLogger",
                                    debugModules = cms.untracked.vstring('*'),
                                    destinations = cms.untracked.vstring('cout'),
                                    cout = cms.untracked.PSet
                                    (
    threshold = cms.untracked.string('DEBUG'),
    noLineBreaks = cms.untracked.bool(True)
    )
                                    )

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.add_(cms.ESProducer("TGeoMgrFromDdd",
        verbose = cms.untracked.bool(False),
        level   = cms.untracked.int32(14)
))

process.dump = cms.EDAnalyzer("DumpSimGeometry")

process.p = cms.Path(process.dump)


