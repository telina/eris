ctmc

const double rn16SEC = 0.001;
const double rn16SAFE = 0.00015;
const double rn16GUAR = 0;
const double rn11SEC = 0.001;
const double rn11SAFE = 0.0001;
const double rn11GUAR = 0;
const double rn13SEC = 0.004;
const double rn13SAFE = 0.0009;
const double rn13GUAR = 0;
const double rn15SEC = 0.001;
const double rn15SAFE = 0.0008;
const double rn15GUAR = 0;
const double rn17SEC = 0;
const double rn17SAFE = 0.0005;
const double rn17GUAR = 0;
const double rn18SEC = 0;
const double rn18SAFE = 0.0005;
const double rn18GUAR = 0;
const double rn10SEC = 0.001;
const double rn10SAFE = 0.0001;
const double rn10GUAR = 0;
const double rn14SEC = 0.001;
const double rn14SAFE = 0.00011;
const double rn14GUAR = 0;
const double rn12SEC = 0.001;
const double rn12SAFE = 0.000013;
const double rn12GUAR = 0;

formula n13essentials = n17=0 & n16=0;
formula n15essentials = n18=0;
formula operational = (n16=0) & (n11=0 | n10=0) & (n15=0 & (n18=0)) & (n10=0 | n11=0) & (n14=0) & (n12=0) ;

module generatedScenario

n16: [0..2] init 0;
n11: [0..2] init 0;
n13: [0..2] init 0;
n15: [0..2] init 0;
n17: [0..2] init 0;
n18: [0..2] init 0;
n10: [0..2] init 0;
n14: [0..2] init 0;
n12: [0..2] init 0;

[] (n16=0) & (operational) -> rn16SAFE : (n16'=1);
[] (n16=0) & (operational) -> rn16SEC : (n16'=2);
[] (n16=2) & (operational) -> rn16SAFE : (n16'=1);
[] (n11=0) & (operational) -> rn11SAFE : (n11'=1);
[] (n11=0) & (operational) -> rn11SEC : (n11'=2);
[] (n11=2) & (operational) -> rn11SAFE : (n11'=1);
[] (n13=0) & (!n13essentials) & (operational)-> (n13'=1);
[] (n13=0) & (operational) -> rn13SAFE : (n13'=1);
[] (n13=0) & (operational) -> rn13SEC : (n13'=2);
[] (n15=0) & (!n15essentials) & (operational)-> (n15'=1);
[] (n15=0) & (operational) -> rn15SAFE : (n15'=1);
[] (n15=0) & (operational) -> rn15SEC : (n15'=2);
[] (n15=2) & (operational) -> rn15SAFE : (n15'=1);
[] (n17=0) & (operational) -> rn17SAFE : (n17'=1);
[] (n18=0) & (operational) -> rn18SAFE : (n18'=1);
[] (n10=0) & (operational) -> rn10SAFE : (n10'=1);
[] (n10=0) & (operational) -> rn10SEC : (n10'=2);
[] (n10=2) & (operational) -> rn10SAFE : (n10'=1);
[] (n14=0) & (operational) -> rn14SAFE : (n14'=1);
[] (n14=0) & (operational) -> rn14SEC : (n14'=2);
[] (n14=2) & (operational) -> rn14SAFE : (n14'=1);
[] (n12=0) & (operational) -> rn12SAFE : (n12'=1);
[] (n12=0) & (operational) -> rn12SEC : (n12'=2);
[] (n12=2) & (operational) -> rn12SAFE : (n12'=1);


endmodule
label "defective" = !operational;
label "safetyfailure" = (n16=1) | (n11=1 & n10=1) | (n15=1 | (n18=1)) | (n10=1 & n11=1) | (n14=1) | (n12=1) ;
label "corrupted" = n16=2 | (n11=2 & n10=2) | n15=2 | (n10=2 & n11=2) | n14=2 | n12=2 ;
