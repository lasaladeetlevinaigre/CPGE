function couleurRandom() {
  // Liste de huit couleurs prédéfinies
  var couleurs = ["#3498db", "#e74c3c", "#2ecc71", "#e6e522", "#9b59b6", "#34495e", "#1abc9c", "#e67e22"];

  for (var i = 0; i < 8; i++) {
    for (var j = 0; j < 8; j++) {

      var index = Math.floor(Math.random() * couleurs.length);


      document.getElementById(""+i+j).style.background = couleurs[index];
      
    }
  }

}