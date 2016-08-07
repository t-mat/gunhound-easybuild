#ifndef _CMENU_H_
#define _CMENU_H_

enum {
	//シナリオ番号
	enScenarioNone = -1,
	enScenarioJungle,
	enScenarioRiver,
	enScenarioMountain,
	enScenarioBase,
	enScenarioIronGear,
	enScenarioTutorial,
	enScenarioMax,
	enScenarioExtra,
	enScenarioExtra2,
	enScenarioExtra3,
};


enum EnScen {
	//シナリオの番号

	enScenNone=-99,
	enScenTerm=-99,
	enScenFirst=-1,
	enScenStart=0,
	enScenR__,
		enScenRM_,
			enScenRMB,
				enScenEnd2,
			enScenRMJ,
				enScenEnd3,

		enScenRB_,
			enScenRBM,
				enScenEnd1,
			enScenRBJ,
				enScenEnd4,

	enScenM__,
		enScenMR_,
			enScenMRB,
				enScenEnd5,
			enScenMRJ,
				enScenEnd6,

		enScenMB_,
			enScenMBJ,
				enScenEnd7,
	enScenExtra,
	enScenMax,
};

enum {
	enClearSeqNone,
	enClearSeqStageClear,
	enClearSeqContinue,
	enClearSeqGameOver,
	enClearSeqEnding,
	enClearSeqReset,
};

#endif
