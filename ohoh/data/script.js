async function fetchData() {
  try {
    const response = await fetch("/GetAllData");
    if (!response.ok) throw new Error(`HTTP Error: ${response.status}`);
    const data = await response.json(); 
    document.getElementById("RealTemp").innerText = data.RealTemp || "--";
    document.getElementById("MinTemp").innerText = data.MinTemp || "--";
    document.getElementById("MaxTemp").innerText = data.MaxTemp || "--";
    document.getElementById("TargetTemp").innerText = data.TargetTemp || "--";
    document.getElementById("CalentadorStatus").innerText = data.CalentadorStatus || "--";
    document.getElementById("HumidiStatus").innerText = data.HumidiStatus || "--";
    document.getElementById("HumidiOn").innerText = data.HumidiOn || "--";
    document.getElementById("HumidiWait").innerText = data.HumidiWait || "--";
    document.getElementById("AirOn").innerText = data.AirOn || "--";
    document.getElementById("AirWait").innerText = data.AirWait || "--";
  } catch (error) {
    console.error("Error:", error);
  }
}
setInterval(fetchData, 10000);
fetchData();


function NubeOn() {
  fetch('/NubeOn', { method: 'GET' })
    .then(response => {
      if (response.ok) {
        fetchData();}})}
function NubeStop() {
  fetch('/NubeStop', { method: 'GET' })
    .then(response => {
      if (response.ok) {
        fetchData();}})}



function TargetTempA() {
  fetch('/TargetTemp+', { method: 'GET' })
    .then(response => {
      if (response.ok) {
        fetchData();}})}
function TargetTempD() {
  fetch('/TargetTemp-', { method: 'GET' })
    .then(response => {
      if (response.ok) {
       fetchData();}})}


function HumidiOnA() {
  fetch('/HumidiOn+', { method: 'GET' })
    .then(response => {
      if (response.ok) {
        fetchData();}})}
function HumidiOnD() {
  fetch('/HumidiOn-', { method: 'GET' })
    .then(response => {
      if (response.ok) {
        fetchData();}})}


function HumidiWaitA() {
  fetch('/HumidiWait+', { method: 'GET' })
    .then(response => {
      if (response.ok) {
        fetchData();}})}
function HumidiWaitD() {
  fetch('/HumidiWait-', { method: 'GET' })
    .then(response => {
      if (response.ok) {
        fetchData();}})}


function AirOnA() {
  fetch('/AirOn+', { method: 'GET' })
    .then(response => {
      if (response.ok) {
        fetchData();}})}
function AirOnD() {
  fetch('/AirOn-', { method: 'GET' })
    .then(response => {
      if (response.ok) {
        fetchData();}})}


function AirWaitA() {
  fetch('/AirWait+', { method: 'GET' })
    .then(response => {
      if (response.ok) {
        fetchData();}})}
function AirWaitD() {
  fetch('/AirWait-', { method: 'GET' })
    .then(response => {
      if (response.ok) {
        fetchData();}})}
		
		function ReSet() {
  fetch('/ReSet', { method: 'GET' })
    .then(response => {
      if (response.ok) {
        fetchData();}})}
