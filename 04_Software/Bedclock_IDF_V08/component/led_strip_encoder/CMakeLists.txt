set(component_srcs "led_strip_encoder.c" )

# get IDF version for comparison
set(idf_version "${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}")

idf_component_register(SRCS "${component_srcs}" PRIV_REQUIRES driver INCLUDE_DIRS ".")

idf_component_register(SRCS "src/led_strip_encoder.c"
                       INCLUDE_DIRS "include")