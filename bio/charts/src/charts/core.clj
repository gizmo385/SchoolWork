(ns charts.core
  (:use [incanter core stats charts io datasets]
        [clojure.pprint]))

(def data
  (to-dataset
    [{:color "red"    :wavelength 400 :absorbance 0.2}
     {:color "red"    :wavelength 450 :absorbance 0.28}
     {:color "red"    :wavelength 500 :absorbance 0.58}
     {:color "red"    :wavelength 550 :absorbance 0.26}
     {:color "red"    :wavelength 600 :absorbance 0.2}
     {:color "red"    :wavelength 650 :absorbance 0}
     {:color "blue"   :wavelength 400 :absorbance 0.2}
     {:color "blue"   :wavelength 450 :absorbance 0}
     {:color "blue"   :wavelength 500 :absorbance 0.04}
     {:color "blue"   :wavelength 550 :absorbance 0.19}
     {:color "blue"   :wavelength 600 :absorbance 0.75}
     {:color "blue"   :wavelength 650 :absorbance 0.95}
     {:color "green"  :wavelength 400 :absorbance 0.85}
     {:color "green"  :wavelength 450 :absorbance 0.65}
     {:color "green"  :wavelength 500 :absorbance 0.09}
     {:color "green"  :wavelength 550 :absorbance 0.2}
     {:color "green"  :wavelength 600 :absorbance 0.7}
     {:color "green"  :wavelength 650 :absorbance 0.85}]))

(comment
  save
  (view (line-chart :wavelength :absorbance :group-by :color :legend true :data data
                    :x-label "Wavelength(nm)" :y-label "Absorbance Ratio"
                    :title "Light Spectrum Absorbance by Colored Dyes"))
  (save
    (line-chart :wavelength :absorbance :group-by :color :legend true :data data
                :x-label "Wavelength(nm)" :y-label "Absorbance Ratio"
                :title "Light Spectrum Absorbance by Colored Dyes")
    "absorbance.png")

  (view  (line-chart  ["a" "b" "c" "d" "e" "f"]  [10 20 30 10 40 20]))
  (view  (line-chart years x :group-by seasons :legend true))
  (view (line-chart wavelengths (concat red green blue) :group-by colors :legend true))
  )
